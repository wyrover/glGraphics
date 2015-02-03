//Primary author: Jonathan Bedard
//Confirmed working: 1/16/2015

#pragma once

#ifndef GLSPECIALFORM_H
#define GLSPECIALFORM_H

using namespace std;

#include "glAdvanced.h"

//Restrict the size of a string (from the back)
static string gl_restrict_size(string source, void* font, int length)
{
	string ret = source;
	int cur_len;

	if(length<=0)
		return "";
	if(glutBitmapLength(font, (unsigned char*)ret.c_str()) < length)
		return ret;

	ret = ret + "...";

	while(glutBitmapLength(font, (unsigned char*)ret.c_str()) > length && ret != "...")
	{
		ret = ret.substr(0,ret.length()-4)+"...";
	}
	return ret;
}
//Restricts the size of a string (from the front)
static string gl_restrict_size_front(string source, void* font, int length)
{
	string ret = source;
	int cur_len;

	if(length<=0)
		return "";
	if(glutBitmapLength(font, (unsigned char*)ret.c_str()) < length)
		return ret;

	ret = "..."+ret;

	while(glutBitmapLength(font, (unsigned char*)ret.c_str()) > length && ret != "...")
	{
		ret = "..."+ret.substr(4,ret.length());
	}
	return ret;
}

class glFileEditForm:
	public glForm
{
protected:
	glScrollbar scrollbar;
	glElement rightDiv;
	glButton backButton;

	glLabel lblHeader;
	glArrowButton back;
	glElement upDiv;

	glButton** fileIDButtons;
	glArrowButton** folderArrows;
	string* good_name_array;

	string currentDirectoryLocation;
	bool must_add;
	int num_files;
	int num_good_files;
	int num_columns;

	int store_height;
	int store_width;

	//Functions
	bool (*screen_address)(string*);
	void (*modify_button)(glButton*,glArrowButton*, string*);
	void (*button_click)(glElement*, string*, glFileEditForm*);

public:
	glFileEditForm(glForm* prev, char* starting_loc);
	virtual ~glFileEditForm();

	//Set Functions
	void setDirectoryLocation(string x);
	void set_screen_function(bool (*func)(string*));
	void set_button_modification_function(void (*func)(glButton*,glArrowButton*, string*));
	void set_button_click_Function(void (*func)(glElement*, string*, glFileEditForm*));

	//Button array
	void recheckFiles();
	void moveButtons();

	//Moving Directories
	bool moveUp(string dir_name);
	bool moveDown();
	
	//Actions
	virtual void virtual_update();
	virtual void virtualFormResize();
	void button_pressed(glElement* source);
};

#endif