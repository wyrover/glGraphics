//Primary author: Jonathan Bedard
//Confirmed working: 2/14/2015

#pragma once

#ifndef GLSPECIALFORM_CPP
#define GLSPECIALFORM_CPP

using namespace std;

#include "glSpecialForm.h"
#include "osGraphics.h"
#include "glOSFunctions.h"

	static void button_event_file_edit_form(glElement* input, void* source)
	{
		glFileEditForm* master = (glFileEditForm*) source;
		master->button_pressed(input);
	}

//Constuctor-----------------------------------------------------------------------------------------------
	//Constructor
	glFileEditForm::glFileEditForm(glForm* prev, char* starting_loc):
		glForm(prev)
	{
		is_local = false;
		store_height = height;
		store_width = width;
		local_files = NULL;
		local_files_len =0;

		//Bad form
		if(width - 120 < 170)
		{
			cerr<<"Form too small to display"<<endl;
			return_form();
			return;
		}
		num_columns = ((width - 145))/170;
		num_good_files = 0;

		//Colors
		color black;
		black.blue = black.green = black.red = 0;
		black.alpha = 1;

		//Scrollbar
		scrollbar.setClickEvent(&button_event_file_edit_form);
		pushElement(&scrollbar);
		addMouseListener(&scrollbar);

		//Back button
		back.setWidth(30);
		back.setHeight(40);
		back.setX(25);
		back.setDirection(IND_LEFT);
		pushElement(&back);
		addMouseListener(&back);
		back.setClickEvent(&button_event_file_edit_form);
		back.setEnterEvent(&button_event_file_edit_form);

		//Header divider
		lblHeader.setX(60);
		//lblHeader.setText((char*)"");
		pushElement(&lblHeader);

		//Upper divider
		upDiv.setColor(black);
		upDiv.setWidth(width - 140);
		upDiv.setHeight(5);
		upDiv.setX(20);
		pushElement(&upDiv);

		//Right divider
		rightDiv.setWidth(5);
		rightDiv.setHeight(height);
		rightDiv.setY(0);
		rightDiv.setX(width - 120);
		rightDiv.setColor(black);
		pushElement(&rightDiv);

		//Back button
		backButton.setText((char*) "Quit");
		backButton.setWidth(100);
		backButton.setX(width-110);
		backButton.setY(10);
		pushElement(&backButton);
		addMouseListener(&backButton);
		backButton.setClickEvent(&button_event_file_edit_form);
		backButton.setEnterEvent(&button_event_file_edit_form);

		fileIDButtons = NULL;
		folderArrows = NULL;
		good_name_array = NULL;

		screen_address = NULL;
		modify_button = NULL;
		button_click = NULL;

		setDirectoryLocation(string(starting_loc));
	}
	//Destructor
	glFileEditForm::~glFileEditForm()
	{
		//Remove old elements
		int cnt = 0;
		if(fileIDButtons != NULL)
		{
			while(cnt<num_files)
			{
				if(fileIDButtons[cnt] != NULL)
				{
					removeElement(fileIDButtons[cnt]);
					delete fileIDButtons[cnt];
				}
				if(folderArrows[cnt] != NULL)
				{
					removeElement(folderArrows[cnt]);
					delete folderArrows[cnt];
				}
				cnt++;
			}
			delete [] fileIDButtons;
			delete [] folderArrows;
			delete [] good_name_array;
		}
	}

//Set Functions----------------------------------------------------------------------------------------------

	void glFileEditForm::setDirectoryLocation(string x)
	{
		currentDirectoryLocation = x;
		if(currentDirectoryLocation == "" || !gl_is_directory(currentDirectoryLocation))
			currentDirectoryLocation = glGetSource();

		recheckFiles();
	}
	void glFileEditForm::set_screen_function(bool (*func)(string*))
	{
		must_add = true;
		screen_address = func;
	}
	void glFileEditForm::set_button_modification_function(void (*func)(glButton*,glArrowButton*, string*))
	{
		must_add = true;
		modify_button = func;
	}
	void glFileEditForm::set_button_click_Function(void (*func)(glElement*, string*, glFileEditForm*))
	{
		button_click = func;
	}
	void glFileEditForm::set_local_files(string* f_array, int len)
	{
		local_files = f_array;
		local_files_len = len;
	}

//Button Arrays---------------------------------------------------------------------------------------------
	
	//Reinitializes all of the buttons
	void glFileEditForm::recheckFiles()
	{
		must_add = false;

		//Set the header label
		if(is_local)
			lblHeader.setText((char*) gl_restrict_size_front(glGetSource().c_str(),lblHeader.getFont(),width-200).c_str());
		else
			lblHeader.setText((char*) gl_restrict_size_front(currentDirectoryLocation.c_str(),lblHeader.getFont(),width-200).c_str());

		//Remove old elements
		int cnt = 0;
		if(fileIDButtons != NULL)
		{
			while(cnt<num_files)
			{
				if(fileIDButtons[cnt] != NULL)
				{
					removeElement(fileIDButtons[cnt]);
					delete fileIDButtons[cnt];
				}
				if(folderArrows[cnt] != NULL)
				{
					removeElement(folderArrows[cnt]);
					delete folderArrows[cnt];
				}
				cnt++;
			}
			delete [] fileIDButtons;
			delete [] folderArrows;
			delete [] good_name_array;
		}

		//Pull files
		num_files = 0;
		num_good_files = 0;
		int new_cnt = 0;
		cnt = 0;
		string* file_array;
		if(is_local)
		{
			file_array = local_files;
			num_files = local_files_len;
		}
		else
			file_array = gl_list_files(currentDirectoryLocation,&num_files);

		fileIDButtons = new glButton*[num_files];
		folderArrows = new glArrowButton*[num_files];
		good_name_array = new string[num_files];

		//Set everything to NULL
		while(cnt<num_files)
		{
			folderArrows[cnt] = NULL;
			fileIDButtons[cnt] = NULL;
			cnt++;
		}
		cnt = 0;
		
		//Place the file buttons
		while(cnt<num_files)
		{
			if((screen_address==NULL || screen_address(&(file_array[cnt])))&&(gl_is_directory(file_array[cnt]) || glGetExecutable()==gl_extract_name(file_array[cnt])))
			{
				good_name_array[new_cnt] = file_array[cnt];
				fileIDButtons[new_cnt] = new glButton();
				string out_put = gl_extract_name(file_array[cnt]);
				fileIDButtons[new_cnt]->setText((char*) gl_restrict_size(out_put,fileIDButtons[new_cnt]->getFont(),140).c_str());
				fileIDButtons[new_cnt]->setClickEvent(&button_event_file_edit_form);
				fileIDButtons[new_cnt]->setEnterEvent(&button_event_file_edit_form);

				pushElement(fileIDButtons[new_cnt]);
				addMouseListener(fileIDButtons[new_cnt]);

				folderArrows[new_cnt] = new glArrowButton();

				folderArrows[new_cnt]->setWidth(15);
				folderArrows[new_cnt]->setHeight(fileIDButtons[new_cnt]->getHeight());
				folderArrows[new_cnt]->setDirection(IND_RIGHT);
				folderArrows[new_cnt]->setClickEvent(&button_event_file_edit_form);
				folderArrows[new_cnt]->setEnterEvent(&button_event_file_edit_form);

				pushElement(folderArrows[new_cnt]);
				addMouseListener(folderArrows[new_cnt]);

				num_good_files++;
				new_cnt++;
			}
			cnt++;
		}
		cnt = 0;
		while(cnt<num_files)
		{
			if(!(gl_is_directory(file_array[cnt]) || glGetExecutable()==gl_extract_name(file_array[cnt])) && (screen_address==NULL || screen_address(&(file_array[cnt]))))
			{
				good_name_array[new_cnt] = file_array[cnt];
				fileIDButtons[new_cnt] = new glButton();
				string out_put = gl_extract_name(file_array[cnt]);
				fileIDButtons[new_cnt]->setText((char*) gl_restrict_size(out_put,fileIDButtons[new_cnt]->getFont(),140).c_str());
				fileIDButtons[new_cnt]->setClickEvent(&button_event_file_edit_form);
				fileIDButtons[new_cnt]->setEnterEvent(&button_event_file_edit_form);

				pushElement(fileIDButtons[new_cnt]);
				addMouseListener(fileIDButtons[new_cnt]);

				folderArrows[new_cnt] = NULL;

				num_good_files++;
				new_cnt++;
			}
			cnt++;
		}

		if(!is_local)
			delete [] file_array;

		int num_rows = (num_good_files+(num_columns - num_good_files%num_columns))/num_columns;
		if(num_good_files%num_columns == 0)
			num_rows = num_good_files/num_columns;
		scrollbar.setExpandedWindowSize(50*num_rows+80);

		moveButtons();
	}
	//Move all of the buttons
	void glFileEditForm::moveButtons()
	{
		int inner_cnt = 0;
		int outer_cnt = 0;
		int total_cnt = 0;

		int vertical_loc;
		int horizontal_loc;
		int num_rows = (num_good_files+(num_columns - num_good_files%num_columns))/num_columns;
		if(num_good_files%num_columns == 0)
			num_rows = num_good_files/num_columns;
		
		//Set header element
		lblHeader.setY(height - 40 + scrollbar.getScrollPosition());
		back.setY(height - 45 + scrollbar.getScrollPosition());
		upDiv.setY(height - 55 + scrollbar.getScrollPosition());

		horizontal_loc = 25;
		while(outer_cnt < num_columns && total_cnt<num_files)
		{
			inner_cnt = 0;
			vertical_loc = height - 105 + scrollbar.getScrollPosition();
			while(inner_cnt < num_rows && total_cnt<num_files)
			{
				if(fileIDButtons[total_cnt] != NULL)
				{
					if(modify_button!=NULL)
						modify_button(fileIDButtons[total_cnt],folderArrows[total_cnt],&(good_name_array[total_cnt]));
					
					fileIDButtons[total_cnt]->setX(horizontal_loc);
					fileIDButtons[total_cnt]->setY(vertical_loc);
				}
				if(folderArrows[total_cnt] != NULL)
				{
					folderArrows[total_cnt]->setX(horizontal_loc+150);
					folderArrows[total_cnt]->setY(vertical_loc);
				}

				vertical_loc-=50;
				inner_cnt++;
				total_cnt++;
			}
			horizontal_loc+=170;
			outer_cnt++;
		}
	}

//Moving Directories----------------------------------------------------------------------------------------
	
	//Move up into a named directory
	bool glFileEditForm::moveUp(string dir_name)
	{
		//The local case: move to that folder
		if(is_local)
		{
			if(!gl_is_directory(dir_name))
				return false;
			currentDirectoryLocation = dir_name;
			is_local = false;
			must_add = true;
			return true;
		}

		//The general case
		string old = currentDirectoryLocation;
		currentDirectoryLocation = old + "/" + dir_name;
		if(gl_is_directory(currentDirectoryLocation))
		{
			must_add = true;
			return true;
		}
		currentDirectoryLocation = old;
		if(dir_name == glGetExecutable())
		{
			must_add = true;
			is_local = true;
			return true;
		}
		return false;
	}
	//Move back a directory
	bool glFileEditForm::moveDown()
	{
		//First, test if we are local
		if(is_local)
		{
			is_local = false;
			must_add = true;
			return true;
		}
		string test_val = gl_extract_name(currentDirectoryLocation);

		//Relative directories
		if(test_val == "." || test_val == "..")
		{
			string new_dir;
			if(test_val == "..")
			{
				int cnt = 0;
				int track_depth = 1;
				while(cnt<currentDirectoryLocation.length())
				{
					if(currentDirectoryLocation.at(cnt) == '/'||currentDirectoryLocation.at(cnt) == '\\')
						track_depth++;
					cnt++;
				}
				if(track_depth >= glGetSourceDepth())
					return false;
				new_dir = currentDirectoryLocation+"\\..";
			}
			else
			{
				new_dir = "." + currentDirectoryLocation;
			}

			if(!gl_is_directory(new_dir))
				return false;
			
			string* temp;
			int num_of = 0;

			temp = gl_list_files(new_dir,&num_of);
			delete [] temp;
			if(num_of <= 0)
				return false;

			currentDirectoryLocation = new_dir;
			must_add = true;

			return true;
		}

		//Regular directories
		if(currentDirectoryLocation.length()-1-test_val.length() <= 0)
			return false;
		string new_dir = currentDirectoryLocation.substr(0,currentDirectoryLocation.length()-1-test_val.length());
		if(glIsBase(new_dir))
			new_dir = new_dir + '\\';
		if(!gl_is_directory(new_dir))
			return false;

		string* temp;
		int num_of = 0;

		temp = gl_list_files(new_dir,&num_of);
		delete [] temp;
		if(num_of <= 0)
			return false;

		currentDirectoryLocation = new_dir;
		must_add = true;

		return true;
	}

//Actions---------------------------------------------------------------------------------------------------
	
	void glFileEditForm::virtual_update()
	{
		if(must_add)
			recheckFiles();
	}
	//Triggered when the form changes size
	void glFileEditForm::virtualFormResize()
	{
		forceSize(store_width,store_height);
	}
	//Triggered during an action
	void glFileEditForm::button_pressed(glElement* source)
	{
		//Back button
		if(source == &back)
		{
			moveDown();
			return;
		}

		//Scrollbar
		if(source == &scrollbar)
		{
			moveButtons();
			return;
		}

		//Back button
		if(source == &backButton)
		{
			return_form();
			return;
		}

		//All buttons
		int cnt = 0;
		while(fileIDButtons!=NULL && cnt<num_files)
		{
			if(source==fileIDButtons[cnt])
			{
				if(button_click != NULL)
					button_click(source,&(good_name_array[cnt]),this);
				return;
			}
			cnt++;
		}

		//Arrow buttons
		cnt = 0;
		while(folderArrows!=NULL && cnt<num_files)
		{
			if(source==folderArrows[cnt])
			{
				if(is_local)
					moveUp(good_name_array[cnt]);
				else
					moveUp(gl_extract_name(good_name_array[cnt]));
				return;
			}
			cnt++;
		}
	}


#endif