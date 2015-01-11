//Primary author: Jonathan Bedard
//Confirmed working: 1/8/2015

#ifndef GLFORM_CPP
#define GLFORM_CPP

#include <stdio.h>

#include "glAdvanced.h"
#include "osGraphics.h"

#include <iostream>
#include <string>



using namespace std;

//Constructor/Destructor---------------------------------------------------------------------------------------------------------------------
	
	//Initializes the form
	glForm::glForm(int argv, char ** addr, int wid, int hei, char* ti,
			void (*dispFunc)(),void (*keyBoard)(unsigned char, int, int),void (*mouse)(int,int,int,int),
			void (*mouseMove)(int,int),void (*kU)(unsigned char, int, int),
			void (*spc_down)(int,int,int),void (*spc_up)(int,int,int))
	{
		next_form = NULL;
		previous_form = NULL;
		vertical_scroller = NULL;
		form_return_flag = false;
		allowTraverse = true;

		width = 100;
		height =100;
		title = (char*) "NULL";
		lockDisplay = true;
		focus = NULL;
		hld = NULL;

		background.red = .9f;
		background.green = .9f;
		background.blue = .9f;
		background.alpha = 1;

		setHeight(hei);
		setWidth(wid);
		setTitle(ti);

		reDisplay=false;

		formBuilder(argv,addr,this,dispFunc,keyBoard,mouse);
		
		glutIgnoreKeyRepeat(1);
		glutPassiveMotionFunc(mouseMove);
		glutMotionFunc(mouseMove);
		
		glutKeyboardUpFunc(kU);
		glutSpecialFunc(spc_up);
		glutSpecialUpFunc(spc_down);
		
		lockDisplay = false;
	}
	//Initializes a 3d form
	glForm3d::glForm3d(int argv, char ** addr, int wid, int hei, char* t,
			float fov,float zn,float zf,
			void (*dispFunc)(),void (*keyBoard)(unsigned char, int, int),void (*mouse)(int,int,int,int),
			void (*mouseMove)(int,int),void (*kU)(unsigned char, int, int),
			void (*spc_down)(int,int,int),void (*spc_up)(int,int,int)):
		glForm(argv, addr, wid, hei, t,*dispFunc,*keyBoard,*mouse,*mouseMove,*kU,*spc_down,*spc_up)
	{
		field_of_view_angle=45;
		z_near=0;
		z_far=100;

		setFOV(fov);
		setZNear(zn);
		setZFar(zf);

		reDisplay=false;
	}
	//Nested form constructor
	glForm::glForm(glForm* prev)
	{
		form_return_flag = false;
		lockDisplay = true;
		allowTraverse = true;
		next_form = NULL;
		previous_form = prev;

		background = prev->getColor();

		setHeight(prev->getHeight());
		setWidth(prev->getWidth());
		setTitle(prev->getTitle());

		focus = NULL;
		hld = NULL;
		vertical_scroller = NULL;

		reDisplay = false;
		lockDisplay = false;
	}
	//Destructors
	glForm::~glForm()
	{
	}
	glForm3d::~glForm3d()
	{}

//Get Functions----------------------------------------------------------------------------------------------------------------------------
	
	//Returns the Width
	int glForm::getWidth()
	{
		return width;
	}
	//Returns the height
	int glForm::getHeight()
	{
		return height;
	}
	//Returns the title
	char* glForm::getTitle()
	{
		return title;
	}
	//Returns the background color
	color glForm::getColor()
	{
		return background;
	}
	//Returns if the form is locked
	bool glForm::getLocked()
	{
		return lockDisplay;
	}
	//Returns the field of view
	float glForm3d::getFOV()
	{
		return field_of_view_angle;
	}
	//Returns Z near variable
	float glForm3d::getZNear()
	{
		return z_near;
	}
	//Returns the Z far variable
	float glForm3d::getZFar()
	{
		return z_far;
	}

//Set Functions----------------------------------------------------------------------------------------------------------------------------

	//Sets the width
	void glForm::setWidth(int x)
	{
		if(x>0)
		{
			width = x;
			reDisplay=false;
		}
	}
	//Sets the height
	void glForm::setHeight(int x)
	{
		if(x>0)
		{
			height = x;
			reDisplay=false;
		}
	}
	//Sets the name
	void glForm::setTitle(char* t)
	{
		if(t!=NULL)
		{
			reDisplay=false;
			title = t;
		}
	}
	//Sets the color
	void glForm::setColor(color x)
	{
		background = x;
	}
	//Sets the lock state of the display
	void glForm::setLocked(bool x)
	{
		lockDisplay = x;
	}
	//Sets the next form
	void glForm::setNextForm(glForm* x)
	{
		is_pop_up = false;
		if(x==NULL)
			form_return_flag = true;
		else
			next_form = x;
	}
	//Sets the pop-up form
	void glForm::setPopUp(glForm* popStr)
	{
		is_pop_up = true;
		if(popStr==NULL)
			form_return_flag = true;
		else
			next_form = (glForm*) popStr;
	}
	//Sets the field of view
	void glForm3d::setFOV(float x)
	{
		if(x>0 && x< 180)
		{
			reDisplay=false;
			field_of_view_angle = x;
		}
	}
	//Sets the Z near variable
	void glForm3d::setZNear(float x)
	{
		if(x>0&&x<z_far)
		{
			reDisplay=false;
			z_near = x;
		}
	}
	//Sets the Z far variable
	void glForm3d::setZFar(float x)
	{
		if(x>z_near)
		{
			reDisplay=false;
			z_far = x;
		}
	}
	//Pushes an element onto the form
	void glForm::pushElement(glElement* x)
	{
		elmList.push_back(x);
		x->setMaster((void*)this);
		if(x->isScroller() == GL_YES)
			vertical_scroller = (glScrollbar*) x;
	}
	//Pushes an element onto the mouse queue
	void glForm::addMouseListener(glElement* x)
	{
		clickEvents.push_back(x);
	}
	//Remove an element
	void glForm::removeElement(glElement* x)
	{
		if(focus == x)
			focus = NULL;
		if(vertical_scroller == x)
			vertical_scroller = NULL;
		elmList.remove(x);
		clickEvents.remove(x);
	}
	//Forces the form to a certain size
	void glForm::forceSize(int x, int y)
	{
		setWidth(x);
		setHeight(y);
		setFormSize(this);
	}
	//Send message to a form
	void glForm::sendMessage(int x)
	{
		//Virtual function
	}
	//Focuses on an element
	void glForm::setFocus(glElement* x)
	{
		traverse_focus_flag = false;
		if(focus!=NULL)
			focus->focus(false);
		for (list<glElement*>::iterator it = clickEvents.begin(); it!=clickEvents.end(); ++it)
		{
			if(x==(*it))
			{
				hld = focus;
				focus = x;
				focus->focus(true);
				if(vertical_scroller!=NULL)
				{
					int cur_pos = 0;
					//Need to move up
					if(focus->getY()+focus->getHeight()+10 > getHeight())
					{
						cur_pos = vertical_scroller->getScrollPosition();
						vertical_scroller->setScrollPosition(cur_pos + (getHeight() - focus->getY()-focus->getHeight()-10));
						vertical_scroller->call_click();
					}

					//Need to move down
					if(cur_pos == 0 && focus->getY()-10 < 0)
					{
						cur_pos = vertical_scroller->getScrollPosition();
						vertical_scroller->setScrollPosition(cur_pos - (focus->getY()-10));
						vertical_scroller->call_click();
					}
				}
				return;
			}
		}
	}
	void glForm::setTraverse(bool x)
	{
		allowTraverse = x;
	}

//Graphics Functions----------------------------------------------------------------------------------------------------------------------
	
	//Initializes the form
	void glForm::initialize()
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, getWidth(), 0, getHeight(), -1, 1);
		glMatrixMode(GL_MODELVIEW);

		glClearColor(background.red, background.green, background.blue, background.alpha);
	}
	//Clears the form before drawing
	void glForm::clear()
	{
		//Test if we need to return to main
		if(form_return_flag && next_form!=NULL)
		{
			delete (next_form);
			form_return_flag = false;
			next_form = NULL;
		}

		if(next_form!=NULL&&!is_pop_up)
		{
			next_form->clear();
			return;
		}
		else if(next_form!=NULL)
			next_form->clear();
		if(!reDisplay)
		{
			initialize();
			formReset(this);
			reDisplay=true;
			formResize();
		}

		checkForceSize(this);
		glMatrixMode(GL_MODELVIEW);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);							// Clear Screen and Depth Buffer
		glLoadIdentity();
	}
	//Draws any background
	void glForm::background_draw()
	{
		if(next_form!=NULL && !is_pop_up)
			next_form->background_draw();
		else
			virtual_background_draw();
	}
	//Virtual background call
	void glForm::virtual_background_draw()
	{
		//Virtual function
	}
	//Draws the (2d) image
	void glForm::draw()
	{
		if(next_form!=NULL && !is_pop_up)
			next_form->draw();
		else
		{
			drawElements();
			virtual_draw();
		}
	}
	//Virtual draw
	void glForm::virtual_draw()
	{
		//Do nothing, virtual function
	}
	//Draws on top of the primary layer
	void glForm::top_draw()
	{
		if(next_form!=NULL && !is_pop_up)
			next_form->top_draw();
		else if(next_form!=NULL)
		{
			virtual_top_draw();

			//Draw entire pop-up
			next_form->background_draw();
			next_form->draw();
			next_form->top_draw();
		}
		else
			virtual_top_draw();
	}
	//Virtual top layer draw
	void glForm::virtual_top_draw()
	{
		//Virtual function
	}
	//Draws the (3d) image
	void glForm3d::draw()
	{
		if(next_form!=NULL)
			next_form->draw();
		else
		{
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			GLfloat aspect = (GLfloat) getWidth() / getHeight();
			gluPerspective(getFOV(), aspect, getZNear(), getZFar());
			glMatrixMode(GL_MODELVIEW);
			glViewport(0, 0, getWidth(), getHeight());


			glPushMatrix();
			td_draw();
			glPopMatrix();

			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();
			glOrtho(0, getWidth(), 0, getHeight(), -1, 1);
			glMatrixMode(GL_MODELVIEW);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glDisable(GL_CULL_FACE);

			glClear(GL_DEPTH_BUFFER_BIT);

			drawElements();
			virtual_draw();
		}
	}
	//Draws the 3d part of the 3d image
	void glForm3d::td_draw()
	{
		//Virtual, no action
	}
	//Swaps out the display
	void glForm::display()
	{
		if(next_form!=NULL)
			next_form->display();
		else
			glutSwapBuffers();
	}
	//Draws the elements
	void glForm::drawElements()
	{
		for (list<glElement*>::iterator it = elmList.begin(); it!=elmList.end(); ++it)
		{
			(*it)->draw();
		}
		for (list<glElement*>::iterator it = elmList.begin(); it!=elmList.end(); ++it)
		{
			(*it)->top_draw();
		}
	}

//Update Functions-------------------------------------------------------------------------------------------------------------------------

	//Updates the form
	void glForm::update()
	{
		if(next_form!=NULL)
			next_form->update();
		else
		{
			hld = NULL;
			virtual_update();
		}
	}
	void glForm::virtual_update()
	{
		//Virtual Function
	}
	//Returns to the previous form
	void glForm::return_form()
	{
		if(previous_form!=NULL)
		{
			previous_form->setNextForm(NULL);
			return;
		}
		else
			exit_form();
	}
	//Exits the form, preforms any cleanup
	void glForm::exit_form()
	{
		//No clean-up needed in a regular form
		exit(0);
	}

//Action Functions------------------------------------------------------------------------------------------------------------------------

	//The function is called whenever the form is resized or its title changes
	void glForm::formResize()
	{
		if(next_form!=NULL)
			next_form->virtualFormResize();
		else
			virtualFormResize();
	}
	//Virtual form resize function
	void glForm::virtualFormResize()
	{
		//Virtual function
	}
	//The mouse event for the entire form
	void glForm::mouse(int button, int state, int mousePositionX, int mousePositionY)
	{
		if(next_form!=NULL)
		{
			next_form->mouse(button, state, mousePositionX, mousePositionY);
			return;
		}
		//Correct moust position
		mousePositionY=height-mousePositionY;
		
		bool clicked = false;

		if(focus!=NULL && focus->mouseIn(mousePositionX,mousePositionY))
		{
			clicked = true;
			focus->clickListener(button, state, mousePositionX, mousePositionY);
		}


		//Check all the clicks
		glElement* temp;
		for (list<glElement*>::iterator it = clickEvents.begin(); it!=clickEvents.end(); ++it)
		{
			if(!clicked)
			{
				clicked = (*it)->clickListener(button, state, mousePositionX, mousePositionY);
				if(clicked)
					focus = (*it);
			}
			else if((*it)!=focus)
				(*it)->clickListener(button, state, ~(-1), ~(-1));
		}

		//Nothing was clicked, focus is NULL
		if(!clicked)
			focus = NULL;

		virtualMouse(button,state,mousePositionX,mousePositionY);
	}
	//The called virtual mouse function
	void glForm::virtualMouse(int button, int state, int mousePositionX, int mousePositionY)
	{
	}
	//The keyboard event for the entire form
	void glForm::keyboard(unsigned char key, int mousePositionX, int mousePositionY)
	{
		if(next_form!=NULL)
		{
			next_form->keyboard(key, mousePositionX, mousePositionY);
			return;
		}
		mousePositionY=height-mousePositionY;

		switch ( key ) 
		{
			case KEY_ESCAPE:        
			exit ( 0 );   
			break;

			default:      
			break;
		}
		
		//Enter key
		if(hld==NULL)
			hld = focus;
		if(hld!=NULL)
		{
			hld->keyListener(key);
			if(hld->getKeyboardStatus())
			{
				if(key == '\n' || key=='\r')
				{
					hld->clickListener(0, 1, ~(-1), ~(-1));
					hld->clickListener(0, 0, ~(-1), ~(-1));
					hld->enter();
					hld->focus(false);
					if(focus==hld)
						focus = NULL;
				}
				hld = NULL;
				return;
			}
		}
		hld = NULL;
		
		virtualKeyboard(key, mousePositionX, mousePositionY);
	}
	//The called virtual keyboard event
	void glForm::virtualKeyboard(unsigned char key, int mousePositionX, int mousePositionY)
	{
	}
	//The mouse motion event
	void glForm::mouseMove(int x, int y)
	{
		if(next_form!=NULL)
		{
			next_form->mouseMove(x, y);
			return;
		}
		y=height-y;
		
		//Unfocus a traverse
		if(traverse_focus_flag)
			setFocus(NULL);
		
		bool focus_elm = false;
		//Test for focus
		if(focus!=NULL&&focus->mouseIn(x,y))
			focus_elm = true;

		//Test all elements
		glElement* temp;
		for (list<glElement*>::iterator it = clickEvents.begin(); it!=clickEvents.end(); ++it)
		{
			if(!focus_elm || focus == (*it))
				(*it)->clickListener(0, -1, x, y);
			else
				(*it)->clickListener(0, -1, ~1, ~1);
		}
		
		virtualMouseMove(x,y);
	}
	//The called virtual mouse motion function
	void glForm::virtualMouseMove(int x, int y)
	{
		//Do nothing
	}
	//Keyboard event, key released
	void glForm::keyUp(unsigned char key, int mousePositionX, int mousePositionY)
	{
		if(next_form!=NULL)
		{
			next_form->keyUp(key, mousePositionX,mousePositionY);
			return;
		}
		mousePositionY=height-mousePositionY;
		//No current action
		virtualKeyUp(key, mousePositionX,mousePositionY);
	}
	//Virtual key released
	void glForm::virtualKeyUp(unsigned char key, int mousePositionX, int mousePositionY)
	{
		//Do nothing
	}
	//Special keys (arrows, ect)
	void glForm::specialKeyDown(int key, int mousePositionX, int mousePositionY)
	{
		if(next_form!=NULL)
		{
			next_form->specialKeyDown(key, mousePositionX,mousePositionY);
			return;
		}
		mousePositionY=height-mousePositionY;

		//Process key press for a focused element
		if(focus!=NULL && focus->surpressSpecial(key))
		{
			focus->specialKeyListener(key);
			if(focus->getKeyboardStatus())
				return;
		}
		
		//Go to first
		if(focus==NULL && allowTraverse && (key == KEY_UP || key == KEY_DOWN ||
				   key == KEY_LEFT || key == KEY_RIGHT))
		{
			for (list<glElement*>::iterator it = clickEvents.begin(); it!=clickEvents.end(); ++it)
			{
				if((*it)->getFocusable())
				{
					setFocus((*it));
					traverse_focus_flag = true;
					return;
				}
			}
		}
		
		//Move "forward"
		if(focus!=NULL && allowTraverse && (key == KEY_DOWN || key == KEY_RIGHT))
		{
			bool flag = false;
			for (list<glElement*>::iterator it = clickEvents.begin(); it!=clickEvents.end(); ++it)
			{
				if((*it)->getFocusable()&&flag)
				{
					setFocus((*it));
					traverse_focus_flag = true;
					return;
				}
				if((*it) == focus)
					flag = true;
			}
			for (list<glElement*>::iterator it = clickEvents.begin(); it!=clickEvents.end(); ++it)
			{
				if((*it)->getFocusable())
				{
					setFocus((*it));
					traverse_focus_flag = true;
					return;
				}
			}
		}
		
		//Move "backward"
		if(focus!=NULL && allowTraverse && (key == KEY_UP || key == KEY_LEFT))
		{
			bool flag = false;
			for (list<glElement*>::reverse_iterator it = clickEvents.rbegin(); it!=clickEvents.rend(); ++it)
			{
				if((*it)->getFocusable()&&flag)
				{
					setFocus((*it));
					return;
				}
				if((*it) == focus)
					flag = true;
			}
			for (list<glElement*>::reverse_iterator it = clickEvents.rbegin(); it!=clickEvents.rend(); ++it)
			{
				if((*it)->getFocusable())
				{
					setFocus((*it));
					return;
				}
			}
		}

		virtualSpecialKeyDown(key, mousePositionX,mousePositionY);
	}
	void glForm::virtualSpecialKeyDown(int key, int mousePositionX, int mousePositionY)
	{
		//Do nothing
	}
	void glForm::specialKeyUp(int key, int mousePositionX, int mousePositionY)
	{
		if(next_form!=NULL)
		{
			next_form->specialKeyUp(key, mousePositionX,mousePositionY);
			return;
		}
		mousePositionY=height-mousePositionY;
		//No current action
		virtualSpecialKeyUp(key, mousePositionX,mousePositionY);
	}
	void glForm::virtualSpecialKeyUp(int key, int mousePositionX, int mousePositionY)
	{
		//Do nothing
	}

//Pop-Up----------------------------------------------------------------------------------------------------------------------------------

	//Pop-up constructor
	glPopUp::glPopUp(glForm* f, char* str):
		glForm(f)
	{
		//Set faded background
		background.blue = .4f;
		background.red = .4f;
		background.green = .4f;
		background.alpha = .6f;

		heightDiv = height/4;
		widthDiv = width/4;

		lblStatus.setText(str);
		pushElement(&lblStatus);

		master_background = f->getColor();
	}
	//Draw the background
	void glPopUp::virtual_background_draw()
	{
		glColor4f(background.red, background.green, background.blue, background.alpha);

		glBegin(GL_QUADS);
		glVertex2f(0,0);
		glVertex2f(0, height);
		glVertex2f(width, height);
		glVertex2f(width, 0);
		glEnd();

		glColor4f(background.red, background.green, background.blue, 1);
		glBegin(GL_QUADS);
		glVertex2f(widthDiv,heightDiv);
		glVertex2f(widthDiv, height-heightDiv);
		glVertex2f(width-widthDiv, height-heightDiv);
		glVertex2f(width-widthDiv, heightDiv);
		glEnd();

		glColor4f(master_background.red, master_background.green, master_background.blue, 1);
		glBegin(GL_QUADS);
		glVertex2f(widthDiv+4,heightDiv+4);
		glVertex2f(widthDiv+4, height-heightDiv-4);
		glVertex2f(width-widthDiv-4, height-heightDiv-4);
		glVertex2f(width-widthDiv-4, heightDiv+4);
		glEnd();

		//Set the location of the status label
		lblStatus.setX(widthDiv+(width-2*widthDiv-lblStatus.getWidth())/2);
		lblStatus.setY(height-heightDiv-40);
	}
	
#endif