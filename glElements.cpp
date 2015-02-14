//Primary author: Jonathan Bedard
<<<<<<< HEAD
//Confirmed working: 2/13/2015
=======
//Confirmed working: 1/14/2015
>>>>>>> origin/master

#ifndef GLELEMENTS_CPP
#define GLELEMENTS_CPP

#include <stdio.h>
#include <iostream>

#include "glAdvanced.h"
#include "osGraphics.h"

#define BORDER_VALUE 3

using namespace std;

//glElement------------------------------------------------------------------------------------------------------------------------------

	//Constructors----------------------------------------------------------

	//Default constructor
	glElement::glElement()
	{
		//Event functions
		clicked = false;
		mouseOver = false;
		master = NULL;
		clickedFunction = NULL;
		pressedFunction = NULL;
		depressedFunction = NULL;
		enterFunction = NULL;
	}
	//Default destructor
	glElement::~glElement()
	{
	}

	//Set Functions---------------------------------------------------------

	//Set X Position
	void glElement::setX(int x)
	{
		xPos = x;
	}
	//Set Y Position
	void glElement::setY(int x)
	{
		yPos = x;
	}
	//Set width
	void glElement::setWidth(int x)
	{
		if(x>=0)
			width = x;
	}
	//Set height
	void glElement::setHeight(int x)
	{
		if(x>=0)
			height = x;
	}
	//Set color
	void glElement::setColor(color c)
	{
		col = c;
	}

	//Get Functions---------------------------------------------------------

	//Get X Position
	int glElement::getX()
	{
		return xPos;
	}
	//Get Y Position
	int glElement::getY()
	{
		return yPos;
	}
	//Get width
	int glElement::getWidth()
	{
		return width;
	}
	//Get height
	int glElement::getHeight()
	{
		return height;
	}
	//Get color
	color glElement::getColor()
	{
		return col;
	}
	//The default keyboard return status
	bool glElement::getKeyboardStatus()
	{
		return false;
	}
	//Returns if an element is focusable
	bool glElement::getFocusable()
	{
		return false;
	}
	//Returns if an element is a scroller
	int glElement::isScroller()
	{
		return GL_NO;
	}

	//Listeners--------------------------------------------------------------
	
	//Sets the function called when the element is clicked
	void glElement::setClickEvent(void (*func) (glElement*,void*))
	{
		clickedFunction = func;
	}
	//Sets the function called when the element is pressed
	void glElement::setPressedEvent(void (*func) (glElement*,void*))
	{
		pressedFunction = func;
	}
	//Sets the function called when the element is released
	void glElement::setReleaseEvent(void (*func) (glElement*,void*))
	{
		depressedFunction = func;
	}
	//Sets the funtion to be called on enter
	void glElement::setEnterEvent(void (*func) (glElement*, void*))
	{
		enterFunction = func;
	}
	//Preforms the enter action
	void glElement::enter()
	{
		if(enterFunction!=NULL)
			enterFunction(this,master);
	}
	//Sets the master of this element
	void glElement::setMaster(void* m)
	{
		master = m;
	}
	//This is the clicked function
	bool glElement::clickListener(int button, int state, int x, int y)
	{
		mouseOver = mouseIn(x,y);
		
		if(button!=0&&state!=-1)
			return false;
		//Pressed
		if(state == 0)
		{
			if(mouseOver)
			{
				clicked = true;
				if(pressedFunction!=NULL)
					pressedFunction(this,master);
				return true;
			}
			return false;
		}
		//Released
		else if(state == 1)
		{
			if(mouseOver)
			{
				if(clicked == true)
				{
					if(depressedFunction!=NULL)
						depressedFunction(this,master);
					if(clickedFunction!=NULL)
						clickedFunction(this,master);
					clicked = false;
				}
			}
			else if(clicked == true)
				state = -1;
		}
		//Possibly moved out
		if(state == -1)
		{
			if(mouseOver)
				return false;
			if(clicked==true)
			{
				clicked = false;
				if(depressedFunction!=NULL)
					depressedFunction(this,master);
			}
		}
		return false;
	}
	//Triggers when any key is pressed
	void glElement::keyListener(char key)
	{
	}
	//Triggers whenever a special key is pressed
	void glElement::specialKeyListener(int key)
	{
	}
	//Surpresses a special key
	bool glElement::surpressSpecial(int key)
	{
		return false;
	}
	//Focuses a glElement (Used for textboxes and key-board owned elements)
	void glElement::focus(bool f)
	{
	}


	//Graphics Functions-----------------------------------------------------

	//Draws the element
	void glElement::draw()
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable( GL_BLEND );

		glColor4f(col.red, col.green, col.blue, col.alpha);
		glBegin(GL_QUADS);
		glVertex2f(xPos, yPos);
		glVertex2f(xPos, yPos + height);
		glVertex2f(xPos + width, yPos + height);
		glVertex2f(xPos + width, yPos);
		glEnd();
	}
	void glElement::top_draw()
	{
		//Virtual function, do nothing
	}
	//Checks if the mouse is in the shape
	bool glElement::mouseIn(int x, int y)
	{
		if(x>xPos&&x<xPos+width&&y>yPos&&y<yPos+height)
			return true;
		return false;
	}

//glLabel-------------------------------------------------------------------------------------------------------------
	
	//Constructor
	glLabel::glLabel()
	{
		//Event functions
		clicked = false;
		mouseOver = false;
		master = NULL;
		clickedFunction = NULL;
		pressedFunction = NULL;
		depressedFunction = NULL;
		
		xPos = 0;
		yPos = 0;

		width = 100;
		height = 30;

		col.red = 1;
		col.blue = 1;
		col.green = 1;
		col.alpha = 0;

		textColor.red = 0;
		textColor.blue = 0;
		textColor.green = 0;
		textColor.alpha = 1;

		centered = false;
		font = GLUT_BITMAP_HELVETICA_18;
		setText(NULL);
		setTextSize();
	}
	//Destructor
	glLabel::~glLabel()
	{
	}
	void glLabel::setTextSize()
	{
		textHeight = 20;
		//textHeight = glutBitmapHeight(font);
		textWidth = glutBitmapLength(font, (unsigned char*)text.c_str());
	}

	//Set Functions--------------------------------------------------------

	//Sets the text
	void glLabel::setText(char* t)
	{
		if(t!=NULL)
			text = string(t);
		else
			text = "NULL";
		setTextSize();
	}
	//Sets the text color
	void glLabel::setTextColor(color c)
	{
		textColor = c;
	}
	//Sets the font
	void glLabel::setFont(void* x)
	{
		font = x;
		setTextSize();
	}
	//Sets if the label is centered
	void glLabel::setCentered(bool x)
	{
		centered = x;
	}

	//Get Functions--------------------------------------------------------

	//Gets the text
	char* glLabel::getText()
	{
		return (char*)text.c_str();
	}
	//Gets the text color
	color glLabel::getTextColor()
	{
		return textColor;
	}
	//Returns the font type
	void* glLabel::getFont()
	{
		return font;
	}
	//Returns if the element is centered
	bool glLabel::getCentered()
	{
		return centered;
	}
	
	//Graphics Functions--------------------------------------------------
	void glLabel::draw()
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable( GL_BLEND );

		glColor4f(col.red, col.green, col.blue, col.alpha);
		glBegin(GL_QUADS);
		glVertex2f(xPos, yPos);
		glVertex2f(xPos, yPos + height);
		glVertex2f(xPos + width, yPos + height);
		glVertex2f(xPos + width, yPos);
		glEnd();

		if(centered)
			drawText(xPos+(width-textWidth)/2,yPos+(height-textHeight)/2,(char*)text.c_str(),textColor,font);
		else
			drawText(xPos+5,yPos+(height-textHeight)/2,(char*)text.c_str(),textColor,font);
	}

//glArrowButton----------------------------------------------------------------

	//Constructor
	glArrowButton::glArrowButton()
	{
		//Event functions
		clicked = false;
		mouseOver = false;
		master = NULL;
		clickedFunction = NULL;
		pressedFunction = NULL;
		depressedFunction = NULL;
		
		xPos = 0;
		yPos = 0;

		width = 20;
		height = 20;
		
		direction = 0;

		h_b = BORDER_VALUE;
		
		col.red = .7f;
		col.green = .7f;
		col.blue = .7f;
		col.alpha = 1;

		textColor.red = 0;
		textColor.blue = 0;
		textColor.green = 0;
		textColor.alpha = 1;
		
		clickedColor.red = .3f;
		clickedColor.blue = 1;
		clickedColor.green = .3f;
		clickedColor.alpha = .7f;
	}
	//Destructor
	glArrowButton::~glArrowButton()
	{
	}
	
	//Set Functions--------------------------------------------------------
	void glArrowButton::setTextColor(color c)
	{
		textColor = c;
	}
	void glArrowButton::setClickedColor(color c)
	{
		clickedColor = c;
	}
	void glArrowButton::setBorderSize(int s)
	{
		if(s<=0)
		{
		      cerr<<"Invalid border size"<<endl;
		      return;
		}
		h_b = s;
	}
	void glArrowButton::setDirection(int d)
	{
		direction = d;
	}
	
	//Get Functions--------------------------------------------------------
	color glArrowButton::getTextColor()
	{
		return textColor;
	}
	color glArrowButton::getClickedColor()
	{
		return clickedColor;
	}
	int glArrowButton::getBorderSize()
	{
		return h_b;
	}
	int glArrowButton::getDirection()
	{
		return direction;
	}
	
	//Graphics Functions---------------------------------------------------
	void glArrowButton::draw()
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable( GL_BLEND );

		if(!clicked&&!mouseOver)
		{
		      glColor4f(col.red, col.green, col.blue, col.alpha);
		      glBegin(GL_QUADS);
		      
		      glVertex2f(xPos, yPos);
		      glVertex2f(xPos, yPos + height);
		      glVertex2f(xPos + width, yPos + height);
		      glVertex2f(xPos + width, yPos);
		      
		      glEnd();
		}
		else if(clicked)
		{
		      glColor4f(clickedColor.red, clickedColor.green, clickedColor.blue, clickedColor.alpha);
		      glBegin(GL_QUADS);
		      
		      glVertex2f(xPos, yPos);
		      glVertex2f(xPos, yPos + height);
		      glVertex2f(xPos + width, yPos + height);
		      glVertex2f(xPos + width, yPos);
		      
		      glEnd();
		}
		else
		{
		      glColor4f(col.red, col.green, col.blue, col.alpha);
		      glBegin(GL_QUADS);
		      
		      //Primary shape
		      glVertex2f(xPos+h_b, yPos+h_b);
		      glVertex2f(xPos+h_b, yPos + height-h_b);
		      glVertex2f(xPos + width-h_b, yPos + height-h_b);
		      glVertex2f(xPos + width-h_b, yPos+h_b);
		      glEnd();
		      
		      //Boundry
		      glColor4f(clickedColor.red, clickedColor.green, clickedColor.blue, clickedColor.alpha);
		      
		      //Right
		      glBegin(GL_QUADS);
		      glVertex2f(xPos, yPos);
		      glVertex2f(xPos, yPos + height);
		      glVertex2f(xPos+h_b, yPos + height-h_b);
		      glVertex2f(xPos+h_b, yPos+h_b);
		      glEnd();
		      
		      //Top
		      glBegin(GL_QUADS);
		      glVertex2f(xPos+h_b, yPos+height-h_b);
		      glVertex2f(xPos, yPos + height);
		      glVertex2f(xPos+width, yPos + height);
		      glVertex2f(xPos+width-h_b, yPos+height-h_b);
		      glEnd();
		      
		      //Left
		      glBegin(GL_QUADS);
		      glVertex2f(xPos+width, yPos);
		      glVertex2f(xPos+width, yPos + height);
		      glVertex2f(xPos+width-h_b, yPos + height-h_b);
		      glVertex2f(xPos+width-h_b, yPos+h_b);
		      glEnd();
		      
		      //Bottom
		      glBegin(GL_QUADS);
		      glVertex2f(xPos+h_b, yPos+h_b);
		      glVertex2f(xPos, yPos);
		      glVertex2f(xPos+width, yPos);
		      glVertex2f(xPos+width-h_b, yPos+h_b);
		      glEnd();
		}
		
		//Indicator
		glColor4f(textColor.red, textColor.green, textColor.blue, textColor.alpha);
		glBegin(GL_TRIANGLES);
		
		if(direction == 0)
		{
			glVertex2f(xPos+width/4, yPos+height/4);
			glVertex2f(xPos+3*width/4, yPos+height/4);
			glVertex2f(xPos+width/2, yPos+3*height/4);
		}
		else if(direction == 1)
		{
			glVertex2f(xPos+width/4, yPos+height/4);
			glVertex2f(xPos+width/4, yPos+3*height/4);
			glVertex2f(xPos+3*width/4, yPos+height/2);
		}
		else if(direction == 2)
		{
			glVertex2f(xPos+width/4, yPos+3*height/4);
			glVertex2f(xPos+3*width/4, yPos+3*height/4);
			glVertex2f(xPos+width/2, yPos+height/4);
		}
		else if(direction == 3)
		{
			glVertex2f(xPos+3*width/4, yPos+height/4);
			glVertex2f(xPos+3*width/4, yPos+3*height/4);
			glVertex2f(xPos+width/4, yPos+height/2);
		}
		
		glEnd();
	}

//glButton---------------------------------------------------------------------

	//Constructor
	glButton::glButton()
	{
		//Event functions
		clicked = false;
		mouseOver = false;
		focused = false;
		master = NULL;
		clickedFunction = NULL;
		pressedFunction = NULL;
		depressedFunction = NULL;
		
		xPos = 0;
		yPos = 0;

		width = 150;
		height = 40;

		h_b = BORDER_VALUE;
		
		col.red = .7f;
		col.green = .7f;
		col.blue = .7f;
		col.alpha = 1;

		textColor.red = 0;
		textColor.blue = 0;
		textColor.green = 0;
		textColor.alpha = 1;
		
		clickedColor.red = .3f;
		clickedColor.blue = 1;
		clickedColor.green = .3f;
		clickedColor.alpha = .7f;

		centered = true;
		font = GLUT_BITMAP_HELVETICA_18;
		setText(NULL);
		setTextSize();
	}
	//Destructor
	glButton::~glButton()
	{}
	
	//Get/Set Functions---------------------------------------------
	
	//Sets the clicked color based on an input
	void glButton::setClickedColor(color c)
	{
		clickedColor = c;
	}
	//Sets the border size
	void glButton::setBorderSize(int s)
	{
		if(s<=0)
		{
		      cerr<<"Invalid border size"<<endl;
		      return;
		}
		h_b = s;
	}
	//Returns the clicked color
	color glButton::getClickedColor()
	{
		return clickedColor;
	}
	//Returns the border size
	int glButton::getBorderSize()
	{
		return h_b;
	}
	//Returns if an element is focusable
	bool glButton::getFocusable()
	{
		return true;
	}
	//Returns that this element does accept keys
	bool glButton::getKeyboardStatus()
	{
		return true;
	}
	
	//Draw Function-------------------------------------------------
	
	void glButton::draw()
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable( GL_BLEND );

		if(!clicked&&!mouseOver&&!focused)
		{
		      glColor4f(col.red, col.green, col.blue, col.alpha);
		      glBegin(GL_QUADS);
		      
		      glVertex2f(xPos, yPos);
		      glVertex2f(xPos, yPos + height);
		      glVertex2f(xPos + width, yPos + height);
		      glVertex2f(xPos + width, yPos);
		      
		      glEnd();
		}
		else if(clicked)
		{
		      glColor4f(clickedColor.red, clickedColor.green, clickedColor.blue, clickedColor.alpha);
		      glBegin(GL_QUADS);
		      
		      glVertex2f(xPos, yPos);
		      glVertex2f(xPos, yPos + height);
		      glVertex2f(xPos + width, yPos + height);
		      glVertex2f(xPos + width, yPos);
		      
		      glEnd();
		}
		else
		{
		      glColor4f(col.red, col.green, col.blue, col.alpha);
		      glBegin(GL_QUADS);
		      
		      //Primary shape
		      glVertex2f(xPos+h_b, yPos+h_b);
		      glVertex2f(xPos+h_b, yPos + height-h_b);
		      glVertex2f(xPos + width-h_b, yPos + height-h_b);
		      glVertex2f(xPos + width-h_b, yPos+h_b);
		      glEnd();
		      
		      //Boundry
		      glColor4f(clickedColor.red, clickedColor.green, clickedColor.blue, clickedColor.alpha);
		      
		      //Right
		      glBegin(GL_QUADS);
		      glVertex2f(xPos, yPos);
		      glVertex2f(xPos, yPos + height);
		      glVertex2f(xPos+h_b, yPos + height-h_b);
		      glVertex2f(xPos+h_b, yPos+h_b);
		      glEnd();
		      
		      //Top
		      glBegin(GL_QUADS);
		      glVertex2f(xPos+h_b, yPos+height-h_b);
		      glVertex2f(xPos, yPos + height);
		      glVertex2f(xPos+width, yPos + height);
		      glVertex2f(xPos+width-h_b, yPos+height-h_b);
		      glEnd();
		      
		      //Left
		      glBegin(GL_QUADS);
		      glVertex2f(xPos+width, yPos);
		      glVertex2f(xPos+width, yPos + height);
		      glVertex2f(xPos+width-h_b, yPos + height-h_b);
		      glVertex2f(xPos+width-h_b, yPos+h_b);
		      glEnd();
		      
		      //Bottom
		      glBegin(GL_QUADS);
		      glVertex2f(xPos+h_b, yPos+h_b);
		      glVertex2f(xPos, yPos);
		      glVertex2f(xPos+width, yPos);
		      glVertex2f(xPos+width-h_b, yPos+h_b);
		      glEnd();
		}
		
		if(centered)
			drawText(xPos+(width-textWidth)/2,yPos+(height-textHeight)/2,getText(),textColor,font);
		else
			drawText(xPos+5,yPos+(height-textHeight)/2,getText(),textColor,font);
	}
	void glButton::focus(bool f)
	{
		focused = f;
	}
	
//glFormMenu------------------------------------------------------------------

	//Nested buttons helper class------------------------
	//Nested button constructor
	nestedButtons::nestedButtons()
	{
		nested_list = NULL;
		master = false;
		isClicked = false;
	}
	//Nested buttons destructor
	nestedButtons::~nestedButtons()
	{
		if(nested_list == NULL)
			return;
		for (list<nestedButtons*>::iterator it = (*nested_list).begin(); it!=(*nested_list).end(); ++it)
		{
			delete((*it));
		}
		delete(nested_list);
	}
	//Sets if this is a master nested button
	void nestedButtons::setMaster(bool x)
	{
		master = x;

		if(nested_list == NULL)
			return;
		for (list<nestedButtons*>::iterator it = (*nested_list).begin(); it!=(*nested_list).end(); ++it)
		{
			(*it)->setMaster(false);
		}
	}
	//Set if the menu has been clicked
	void nestedButtons::setClicked(bool x)
	{
		if(x==true && isClicked==false)
			isClicked = true;
		else
			isClicked = false;

		if(nested_list == NULL || isClicked)
			return;

		for (list<nestedButtons*>::iterator it = (*nested_list).begin(); it!=(*nested_list).end(); ++it)
		{
			(*it)->setClicked(false);
		}
	}
	//Set the location of the menu
	void nestedButtons::setLocation()
	{
		if(nested_list == NULL)
			return;

		//Find longest
		int longest = 0;
		for (list<nestedButtons*>::iterator it = (*nested_list).begin(); it!=(*nested_list).end(); ++it)
		{
			if((*it)->btn.getWidth()>longest)
				longest = (*it)->btn.getWidth();
		}

		int heightTrace;

		if(master)
			heightTrace = btn.getY()-btn.getHeight();
		else
			heightTrace = btn.getY();

		//Set variables
		for (list<nestedButtons*>::iterator it = (*nested_list).begin(); it!=(*nested_list).end(); ++it)
		{
			(*it)->btn.setColor(btn.getColor());
			(*it)->btn.setClickedColor(btn.getClickedColor());

			(*it)->btn.setWidth(longest);
			(*it)->btn.setHeight(btn.getHeight());
			if(master)
				(*it)->btn.setX(btn.getX());
			else
				(*it)->btn.setX(btn.getX()+btn.getWidth());
			(*it)->btn.setY(heightTrace);

			(*it)->setLocation();

			heightTrace=heightTrace-btn.getHeight();
		}
	}
	//Set the master pointer of the interior buttons
	void nestedButtons::setMasterPointer(void* m)
	{
		btn.setMaster(m);
		if(nested_list == NULL)
			return;
		for (list<nestedButtons*>::iterator it = (*nested_list).begin(); it!=(*nested_list).end(); ++it)
			(*it)->setMasterPointer(m);
	}
	//Figure out if the menu is expanded
	bool nestedButtons::getIsClicked()
	{
		return isClicked;
	}
	//Draws the buttons
	void nestedButtons::draw()
	{
		if(isClicked)
		{
			if(nested_list == NULL)
				return;
			int trc = 0;
			for (list<nestedButtons*>::iterator it = (*nested_list).begin(); it!=(*nested_list).end(); ++it)
			{
				(*it)->btn.draw();
				(*it)->draw();
			}
		}
	}
	//Sends mouse commands through the lists
	bool nestedButtons::cascadeClick(int button, int state, int x, int y)
	{
		if(!isClicked)
			return false;
		if(nested_list==NULL)
			return false;

		bool found_clicked = false;
		nestedButtons* comp = NULL;

		list<nestedButtons*>::iterator it = (*nested_list).begin();
		while ( it!=(*nested_list).end() && !found_clicked)
		{
			found_clicked = (*it)->btn.clickListener(button,state,x,y);
			if(found_clicked)
			{
				(*it)->setClicked(true);
				comp = (*it);
			}
			else
			{
				found_clicked = (*it)->cascadeClick(button, state,x,y);
				if(found_clicked)
					comp = (*it);
			}

			++it;
		}

		//Check for closing the dropdowns
		if(state==0 && button == 0)
		{
			//Close all of them
			if(comp==NULL)
			{
				for (list<nestedButtons*>::iterator it = (*nested_list).begin(); it!=(*nested_list).end(); ++it)
				{
					(*it)->setClicked(false);
				}
			}
			//Close the ones not clicked
			else
			{
				for (list<nestedButtons*>::iterator it = (*nested_list).begin(); it!=(*nested_list).end(); ++it)
				{
					if((*it)!=comp)
						(*it)->setClicked(false);
				}
			}
		}

		return found_clicked;
	}
	//Test if the mouse is in the buttons
	bool nestedButtons::mouseIn(int x, int y)
	{
		if(nested_list==NULL || !isClicked)
			return false;

		for (list<nestedButtons*>::iterator it = (*nested_list).begin(); it!=(*nested_list).end(); ++it)
		{
			if((*it)->btn.mouseIn(x,y))
				return true;
			if((*it)->mouseIn(x,y))
				return true;
		}
		return false;
	}

	//Primary Class--------------------------------------
	//Constructor
	glFormMenu::glFormMenu()
	{
		//Event functions
		clicked = false;
		mouseOver = false;
		master = NULL;
		clickedFunction = NULL;
		pressedFunction = NULL;
		depressedFunction = NULL;
		
		//Set color
		col.red = .7f;
		col.green = .7f;
		col.blue = .7f;
		col.alpha = 1;
		
		button_list = NULL;
		invisible = false;
	}
	//Destructor
	glFormMenu::~glFormMenu()
	{
		if(button_list = NULL)
			return;
		for (list<nestedButtons*>::iterator it = (*button_list).begin(); it!=(*button_list).end(); ++it)
		{
			delete((*it));
		}
		delete(button_list);
	}
	//Draw the menu
	void glFormMenu::draw()
	{
		//Do nothing, all drawing needs to be over the top
	}
	//Draws the top layer of the menu
	void glFormMenu::top_draw()
	{
		if(master==NULL)
			return;

		static glForm* form_data = NULL;
		if(form_data!=master)
		{
			form_data = (glForm*) master;
			force_resize();
		}

		bool isDown = false;
		for (list<nestedButtons*>::iterator it = (*button_list).begin(); it!=(*button_list).end()&&!isDown; ++it)
			isDown = (*it)->getIsClicked();

		if(!mouseOver&&!isDown&&invisible)
			return;
		
		//Draw the primary bar
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable( GL_BLEND );

		glColor4f(col.red, col.green, col.blue, col.alpha);
		glBegin(GL_QUADS);
		glVertex2f(xPos, yPos);
		glVertex2f(xPos, yPos + height);
		glVertex2f(xPos + width, yPos + height);
		glVertex2f(xPos + width, yPos);
		glEnd();
		
		if(button_list == NULL)
			return;
		int trc = 0;
		for (list<nestedButtons*>::iterator it = (*button_list).begin(); it!=(*button_list).end(); ++it)
		{
			(*it)->btn.draw();
			(*it)->draw();
			glColor4f(0, 0, 0, col.alpha);
			trc = trc+(*it)->btn.getWidth()+BORDER_VALUE;
			
			glBegin(GL_QUADS);
			glVertex2f(trc, yPos+BORDER_VALUE);
			glVertex2f(trc, yPos + height-BORDER_VALUE);
			glVertex2f(trc - BORDER_VALUE/2, yPos + height-BORDER_VALUE);
			glVertex2f(trc - BORDER_VALUE/2, yPos+BORDER_VALUE);
			glEnd();
		}
	}
	//Resizes the drop down menu
	void glFormMenu::force_resize()
	{
		if(master==NULL)
			return;
		static glForm* form_data = (glForm*) master;

		width = form_data->getWidth();
		height = 40;
		
		xPos = 0;
		yPos = form_data->getHeight()-height;
			
		//Set the location of the buttons
		int trc = 0;
		if(button_list != NULL)
		{
			color clickedColor;
			clickedColor.red = .3f;
			clickedColor.blue = 1;
			clickedColor.green = .3f;
			clickedColor.alpha = 1;
			for (list<nestedButtons*>::iterator it = (*button_list).begin(); it!=(*button_list).end(); ++it)
			{
				(*it)->btn.setX(trc);
				(*it)->btn.setY(yPos);
				(*it)->btn.setColor(col);
				(*it)->btn.setClickedColor(clickedColor);
				(*it)->btn.setHeight(height);
				trc = trc+(*it)->btn.getWidth()+3*BORDER_VALUE/2;
				(*it)->setLocation();
			}
		}
	}

//Set Functions---------------------------------------------------------------

	//Sets the nested buttons
	void glFormMenu::setNestedButtons(list<nestedButtons*>* buttons)
	{
		button_list = buttons;
		if(buttons==NULL)
			return;
		for (list<nestedButtons*>::iterator it = (*button_list).begin(); it!=(*button_list).end(); ++it)
		{
			(*it)->setMaster(true);
		}
		setMaster(master);
	}
	void glFormMenu::setMaster(void* m)
	{
		master = m;
		if(button_list == NULL)
			return;
		for (list<nestedButtons*>::iterator it = (*button_list).begin(); it!=(*button_list).end(); ++it)
			(*it)->setMasterPointer(m);
	}
	//Trigger the clicked listener for the glFormMenu	
	bool glFormMenu::clickListener(int button, int state, int x, int y)
	{
		if(x>xPos&&x<xPos+width&&y>yPos&&y<yPos+height)
			mouseOver = true;
		else
			mouseOver = false;

		bool found_clicked = false;

		if(button_list==NULL)
			return false;

		//Find the button that has been clicked
		list<nestedButtons*>::iterator it = (*button_list).begin();
		nestedButtons* elm = NULL;
		while ( it!=(*button_list).end() && !found_clicked)
		{
			found_clicked = (*it)->btn.clickListener(button,state,x,y);
			if(found_clicked)
			{
				(*it)->setClicked(true);
				elm = (*it);
			}
			else
			{
				found_clicked = (*it)->cascadeClick(button, state,x,y);
				if(found_clicked)
					elm = (*it);
			}
			++it;
		}

		//Check for closing the dropdowns
		if(state==0 && button == 0)
		{
			//Close all of them
			if(elm==NULL)
			{
				for (list<nestedButtons*>::iterator it = (*button_list).begin(); it!=(*button_list).end(); ++it)
				{
					(*it)->setClicked(false);
				}
			}
			else
			{
				for (list<nestedButtons*>::iterator it = (*button_list).begin(); it!=(*button_list).end(); ++it)
				{
					if((*it)!=elm)
						(*it)->setClicked(false);
				}
			}
		}
		
		return found_clicked;
	}
	//Closes the drop down menu
	void glFormMenu::setClosed()
	{
		for (list<nestedButtons*>::iterator it = (*button_list).begin(); it!=(*button_list).end(); ++it)
			(*it)->setClicked(false);
	}
	//The specific colision function for an expanded 
	bool glFormMenu::mouseIn(int x, int y)
	{
		//Check bar
		if(x>xPos&&x<xPos+width&&y>yPos&&y<yPos+height)
			return true;

		if(button_list==NULL)
			return false;

		//Run through nested button list
		for (list<nestedButtons*>::iterator it = (*button_list).begin(); it!=(*button_list).end(); ++it)
		{
			if((*it)->btn.mouseIn(x,y))
				return true;
			if((*it)->mouseIn(x,y))
				return true;
		}

		return false;
	}
	//A hidden menu is not displayed unless the mouse is over it
	void glFormMenu::setHidden(bool x)
	{
		invisible = x;
	}

//glTextbox-------------------------------------------------------------------------------------------------------------
	
	//Constructor
	glTextbox::glTextbox()
	{
		//Event functions
		clicked = false;
		mouseOver = false;
		master = NULL;
		clickedFunction = NULL;
		pressedFunction = NULL;
		depressedFunction = NULL;
		
		xPos = 0;
		yPos = 0;

		width = 150;
		height = 40;

		curse_blink = 0;
		display = true;

		col.red = .7f;
		col.green = .7f;
		col.blue = .7f;
		col.alpha = 1;

		textColor.red = 0;
		textColor.blue = 0;
		textColor.green = 0;
		textColor.alpha = 1;

		clickedColor.red = .3f;
		clickedColor.blue = 1;
		clickedColor.green = .3f;
		clickedColor.alpha = .5f;

		textHeight = 20;

		font = GLUT_BITMAP_HELVETICA_18;
		text = "";
		true_text = string("");
		cursor_loc = -1;
		old_curs = -1;

		max_chars = (-1)>>1;

		pushBadInput('\0');
		pushBadInput('\n');
		pushBadInput('\r');
		pushBadInput('\t');
	}
	//Destructor
	glTextbox::~glTextbox()
	{
	}
	int glTextbox::setCurseLoc(int x)
	{
		curse_blink = 0;

		
		//Fringe cases
		if(x<(xPos+10))
			return 0;
		if(x>(xPos+10+glutBitmapLength(font, (unsigned char*)text.c_str())))
			return text.length();
		
		int old_loc;
		int new_loc = xPos+10;
		int cnt = 0;
		char* tst_array = new char[text.length()+1];

		while(cnt<text.length())
		{
			int cnt2 = 0;
			//Copy the string into a temp array
			while(cnt2<cnt+1)
			{
				tst_array[cnt2] = text.at(cnt2);
				cnt2++;
			}
			tst_array[cnt2] = '\0';

			old_loc = new_loc;
			new_loc =xPos+10+glutBitmapLength(font,(unsigned char*) tst_array);
			 
			if(x<old_loc+(new_loc-old_loc)/2)
			{
				delete[] tst_array;
				return cnt;
			}

			cnt++;
		}
		delete[] tst_array;
		return text.length();
	}
	//The click listener for the textbox
	bool glTextbox::clickListener(int button, int state, int x, int y)
	{
		mouseOver = mouseIn(x,y);
		
		if(button!=0&&state!=-1)
			return false;
		//Pressed
		if(state == 0)
		{
			if(mouseOver)
			{
				clicked = true;
				
				cursor_loc = setCurseLoc(x);
				old_curs = cursor_loc;

				if(pressedFunction!=NULL)
					pressedFunction(this,master);
				return true;
			}
			if(cursor_loc>-1)
			{
				if(clickedFunction!=NULL)
					clickedFunction(this,master);
			}

			if(old_curs !=-1)
				glutIgnoreKeyRepeat(true);
			cursor_loc = -1;
			old_curs = -1;
			return false;
		}
		//Released
		else if(state == 1)
		{
			if(mouseOver)
			{
				if(clicked == true)
				{
					if(depressedFunction!=NULL)
						depressedFunction(this,master);
					clicked = false;
				}
			}
			else if(clicked == true)
			{
				clicked = false;
				if(depressedFunction!=NULL)
						depressedFunction(this,master);
				return true;
			}
		}
		//Possibly moved out
		if(state == -1)
		{
			if(old_curs!=-1&&clicked==true)
				cursor_loc = setCurseLoc(x);
		}
		return false;
	}

	//Set Functions--------------------------------------------------------

	//Sets the text
	void glTextbox::setText(char* t)
	{
		if(t!=NULL)
		{
			true_text = string(t);
			if(display)
				text = true_text;
			else
			{
				string str = "";
				int cnt = 0;
				while(cnt<true_text.length())
				{
					str = str+'*';
					cnt++;
				}
				text = str;
			}
		}
		else
		{
			true_text = "";
			text = "";
		}

		//Ensure that curs_loc bounds are correct
		if(cursor_loc>text.length())
			cursor_loc = -1;
	}
	//Sets the text color
	void glTextbox::setTextColor(color c)
	{
		textColor = c;
	}
	//Sets the font
	void glTextbox::setFont(void* x)
	{
		font = x;
	}
	//Sets the maximum number of characters in a string
	void glTextbox::setMaxChars(int x)
	{
		if(x<=0)
			return;
		max_chars = x;

		if(text.length()>x)
			text = text.substr(0,x);
	}
	void glTextbox::setDisplayType(bool x)
	{
		display = x;
	}
	//Push a bad character onto the list
	void glTextbox::pushBadInput(char x)
	{
		bad_inputs.push_back(x);
	}
	//Sets the clicked color
	void glTextbox::setClickedColor(color c)
	{
		clickedColor = c;
	}
	//Focuses the textbox
	void glTextbox::focus(bool f)
	{
		if(!f)
		{
			if(old_curs !=-1)
				glutIgnoreKeyRepeat(true);
			cursor_loc = -1;
			old_curs = -1;
			if(clickedFunction!=NULL)
					clickedFunction(this,master);
			return;
		}

		cursor_loc = setCurseLoc(xPos+getWidth()-1);
		old_curs = cursor_loc;

		if(pressedFunction!=NULL)
			pressedFunction(this,master);
		if(depressedFunction!=NULL)
			depressedFunction(this,master);
	}

	//Get Functions--------------------------------------------------------

	//Gets the text
	char* glTextbox::getText()
	{
		return (char*) true_text.c_str();
	}
	//Gets the text color
	color glTextbox::getTextColor()
	{
		return textColor;
	}
	//Returns the font type
	void* glTextbox::getFont()
	{
		return font;
	}
	//The special keyboard return status
	bool glTextbox::getKeyboardStatus()
	{
		return true;
	}
	//Returns the clicked color
	color glTextbox::getClickedColor()
	{
		return clickedColor;
	}
	//Returns if an element is focusable
	bool glTextbox::getFocusable()
	{
		return true;
	}

	//Graphics Functions--------------------------------------------------
	void glTextbox::draw()
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable( GL_BLEND );

		glColor4f(col.red, col.green, col.blue, col.alpha);
		glBegin(GL_QUADS);
		glVertex2f(xPos, yPos);
		glVertex2f(xPos, yPos + height);
		glVertex2f(xPos + width, yPos + height);
		glVertex2f(xPos + width, yPos);
		glEnd();

		glColor4f(1, 1, 1, 1);
		glBegin(GL_QUADS);
		glVertex2f(xPos+4, yPos+4);
		glVertex2f(xPos+4, yPos + height-4);
		glVertex2f(xPos + width-4, yPos + height-4);
		glVertex2f(xPos + width-4, yPos+4);
		glEnd();

		//Draw text
		drawText(xPos+10,yPos+(height-textHeight)/2,(char*) text.c_str(),textColor,font);
		
		//Draw cursor
		if(cursor_loc == -1)
			return;

		glutIgnoreKeyRepeat(false);

		glColor4f(0, 0, 0, 1);
		int trace = xPos+10;

		char* array = new char[cursor_loc+1];
		int cnt = 0;
		while(cnt<cursor_loc)
		{
			array[cnt] = text.at(cnt);
			cnt++;
		}
		array[cursor_loc] = '\0';
		if(cursor_loc!=0)
			trace = trace + glutBitmapLength(font, (unsigned char*)array);
		delete[] array;

		curse_blink = (curse_blink+1)%60;

		if(curse_blink<30)
		{
			glBegin(GL_QUADS);
			glVertex2f(trace-1, yPos+8);
			glVertex2f(trace-1, yPos + height-8);
			glVertex2f(trace+1, yPos + height-8);
			glVertex2f(trace+1, yPos+8);
			glEnd();
		}

		//Draw highlighted portion
		if(old_curs == cursor_loc || old_curs<0)
			return;
		glColor4f(clickedColor.red, clickedColor.green, clickedColor.blue, clickedColor.alpha);

		glBegin(GL_QUADS);
		glVertex2f(trace, yPos+8);
		glVertex2f(trace, yPos + height-8);

		trace = xPos+10;
		array = new char[old_curs+1];
		cnt = 0;
		while(cnt<old_curs)
		{
			array[cnt] = text.at(cnt);
			cnt++;
		}
		array[old_curs] = '\0';
		if(old_curs!=0)
			trace = trace + glutBitmapLength(font, (unsigned char*)array);
		delete[] array;

		glVertex2f(trace, yPos + height-8);
		glVertex2f(trace, yPos+8);
		glEnd();
	}
	//Triggers when a normal key is pressed
	void glTextbox::keyListener(char key)
	{
		//Screen out bad characters
		for (list<char>::iterator it = bad_inputs.begin(); it!=bad_inputs.end(); ++it)
		{
			if((*it)==key)
				return;
		}

		//Backspace
		if(key == 8)
		{
			if(cursor_loc<0)
				return;
			if(cursor_loc == old_curs || old_curs == -1)
			{
				if(cursor_loc == 0)
					return;
				true_text = true_text.substr(0,cursor_loc-1)+true_text.substr(cursor_loc,true_text.length()-cursor_loc);
				cursor_loc--;
				old_curs = cursor_loc;
				setText((char*)true_text.c_str());
				return;
			}
			else
			{
				int hi;
				int lo;

				//Find the larger element
				if(cursor_loc>old_curs)
				{
					hi = cursor_loc;
					lo = old_curs;
				}
				else
				{
					hi = old_curs;
					lo = cursor_loc;
				}

				//Delete what's inbetween
				true_text = true_text.substr(0,lo) + true_text.substr(hi,true_text.length());
				cursor_loc = lo;
				old_curs = lo;
				setText((char*)true_text.c_str());
				return;
			}
		}

		//Copy and paste
		if(key == 3 || key == 24 || key == 26)
			return;

		//Out of bounds
		if(xPos+width - 10 < glutBitmapLength(font, (unsigned char*)text.c_str())+10+xPos || text.length() >=max_chars)
		{
			if(cursor_loc == old_curs || old_curs == -1)
				return;
		}

		//Delete highlighted parts
		if(cursor_loc != old_curs && old_curs != -1)
		{
			int hi;
			int lo;

			//Find the larger element
			if(cursor_loc>old_curs)
			{
				hi = cursor_loc;
				lo = old_curs;
			}
			else
			{
				hi = old_curs;
				lo = cursor_loc;
			}

			//Delete what's inbetween
			true_text = true_text.substr(0,lo) + true_text.substr(hi,true_text.length());
			cursor_loc = lo;
			old_curs = lo;
		}

		//Normal type case
		if(true_text.length()>0)
			true_text = true_text.substr(0,cursor_loc)+key+true_text.substr(cursor_loc,true_text.length()-cursor_loc);
		else
			true_text = string("")+key;
		cursor_loc++;
		old_curs = cursor_loc;
		setText((char*)true_text.c_str());
		return;
	}
	//Triggers when a special key is pressed
	void glTextbox::specialKeyListener(int key)
	{
		if(key==KEY_LEFT && cursor_loc>0)
		{
			cursor_loc--;
			old_curs = cursor_loc;
		}
		if(key==KEY_RIGHT && cursor_loc<text.length())
		{
			cursor_loc++;
			old_curs = cursor_loc;
		}
	}
	bool glTextbox::surpressSpecial(int key)
	{
		if(key==KEY_LEFT || key == KEY_RIGHT)
			return true;
		return false;
	}
	
//glScrollbar------------------------------------------------------------------

	//Constructor
	glScrollbar::glScrollbar()
	{
		//Event functions
		clicked = false;
		mouseOver = false;
		master = NULL;
		clickedFunction = NULL;
		pressedFunction = NULL;
		depressedFunction = NULL;
		enterFunction = NULL;
		
		up.setX(0);
		up.setDirection(IND_UP);
		up.setMaster(this);
		
		down.setX(0);
		down.setY(0);
		down.setDirection(IND_DOWN);
		up.setMaster(this);
		
		width = 20;
		xPos = 0;
		yPos = up.getHeight();

		expanded_window = 0;
		scroll_position = 0;
		
		selected = false;
		prev_pos = 0;

		clickedColor.red = .3f;
		clickedColor.blue = 1;
		clickedColor.green = .3f;
		clickedColor.alpha = .7f;

		background.red = .8f;
		background.green = .8f;
		background.blue = .8f;
		background.alpha = 1;

		col.red = .7f;
		col.green = .7f;
		col.blue = .7f;
		col.alpha = 1;
	}
	//Destructor
	glScrollbar::~glScrollbar()
	{
	}

	//Set Functions-------------------------------------------------------
	void glScrollbar::setExpandedWindowSize(int x)
	{
		if(x>=0)
			expanded_window = x;
		else
			x = 0;

		setScrollPosition(scroll_position);
	}
	void glScrollbar::setBackgroundColor(color c)
	{
		background = c;
	}
	void glScrollbar::setScrollPosition(int s)
	{
		scroll_position = s;
		if(scroll_position<0 || master==NULL || height<20)
		{
			scroll_position = 0;
			return;
		}
		
		if(scroll_position>expanded_window-((glForm*)master)->getHeight())
			scroll_position = expanded_window-((glForm*)master)->getHeight();
		if(scroll_position<0)
			scroll_position = 0;
	}

	//Get Functions-------------------------------------------------------
	int glScrollbar::getExpandedWindowSize()
	{
		return expanded_window;
	}
	color glScrollbar::getBackgroundColor()
	{
		return background;
	}
	glArrowButton* glScrollbar::getUp()
	{
		return &up;
	}
	glArrowButton* glScrollbar::getDown()
	{
		return &down;
	}
	int glScrollbar::getScrollPosition()
	{
		return scroll_position;
	}
	int glScrollbar::isScroller()
	{
		return GL_YES;
	}

	//Graphics Functions--------------------------------------------------
	
	void glScrollbar::draw()
	{
		if(master==NULL)
			return;
		up.setY(((glForm*)master)->getHeight()-up.getHeight());
		height = ((glForm*)master)->getHeight()-up.getHeight()-down.getHeight();
		
		if(height<20)
		{
			cerr<<"The form is too small for a scrollbar!"<<endl;
			return;
		}
		
		//Draw buttons
		up.draw();
		down.draw();
		
		//Draw background
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable( GL_BLEND );

		glColor4f(background.red, background.green, background.blue, background.alpha);
		glBegin(GL_QUADS);
		glVertex2f(xPos, yPos);
		glVertex2f(xPos, yPos + height);
		glVertex2f(xPos + width, yPos + height);
		glVertex2f(xPos + width, yPos);
		glEnd();

		//Draw slider
		int true_window_size = expanded_window;
		if(true_window_size<((glForm*)master)->getHeight())
			true_window_size = ((glForm*)master)->getHeight();

		glColor4f(col.red, col.green, col.blue, col.alpha);
		glBegin(GL_QUADS);

		//Calculate scroll bar
		int scroll_size = ((glForm*)master)->getHeight()*(height-4)/true_window_size;
		int scroll_pos = (height-4)*(scroll_position)/true_window_size;
		glVertex2f(xPos, yPos + height - 2 - scroll_pos);
		glVertex2f(xPos, yPos + height - 2 - scroll_size - scroll_pos);
		glVertex2f(xPos + width, yPos + height - 2 - scroll_size - scroll_pos);
		glVertex2f(xPos + width, yPos + height - 2 - scroll_pos);
		glEnd();

		if(selected)
		{
			glColor4f(clickedColor.red, clickedColor.green, clickedColor.blue, clickedColor.alpha);
			glBegin(GL_QUADS);
			glVertex2f(xPos, yPos + height - 2 - scroll_pos);
			glVertex2f(xPos, yPos + height - 2 - scroll_size - scroll_pos);
			glVertex2f(xPos + width, yPos + height - 2 - scroll_size - scroll_pos);
			glVertex2f(xPos + width, yPos + height - 2 - scroll_pos);
		}

	}
	
	//Action Functions---------------------------------------------------
	
	bool glScrollbar::clickListener(int button, int state, int x, int y)
	{
		//Test sizes first
		if(master==NULL)
			return false;
		if(((glForm*)master)->getHeight()-up.getHeight()-down.getHeight()<20)
			return false;
		up.setY(((glForm*)master)->getHeight()-up.getHeight());
		height = ((glForm*)master)->getHeight()-up.getHeight()-down.getHeight();
		
		//Click buttons
		if(up.clickListener(button,state,x,y))
		{
			scrollUp();
			if(clickedFunction!=NULL)
				clickedFunction(this,master);

			return true;
		}
		if(down.clickListener(button,state,x,y))
		{
			scrollDown();
			if(clickedFunction!=NULL)
						clickedFunction(this,master);
			return true;
		}
		
		//Scrolling
		if(button == 3)
		{
			scrollUp();
			if(clickedFunction!=NULL)
					clickedFunction(this,master);
			return false;
		}
		if(button == 4)
		{
			scrollDown();
			if(clickedFunction!=NULL)
					clickedFunction(this,master);
			return false;
		}

		//Click
		if(button!=0)
			return false;
		
		if(master==NULL)
				return false;
		
		if(height<20)
			return false;
		
		int true_window_size = expanded_window;
		if(true_window_size<((glForm*)master)->getHeight())
			true_window_size = ((glForm*)master)->getHeight();
			
		int scroll_size = ((glForm*)master)->getHeight()*(height-4)/true_window_size;
		int scroll_pos = (height-4)*(scroll_position)/true_window_size;

		//Pressed button
		if(state == 0)
		{
			if(x>xPos && x<xPos+width && y<(yPos + height - 2 - scroll_pos) && y>(yPos + height - 2 - scroll_pos - scroll_size))
			{
				prev_pos = y;
				selected = true;
				return false;
			}
		}
		//Released button
		else if(state == 1)
		{
			selected = false;
			prev_pos = 0;
			return true;
		}
		else if(state == -1 && selected == true)
		{
			int dif = prev_pos - y;
			dif = dif*true_window_size/(height-4);
			setScrollPosition(getScrollPosition()+dif);
			prev_pos = y;
			if(clickedFunction!=NULL)
					clickedFunction(this,master);
			return false;
		}

		return false;
	}
	void glScrollbar::call_click()
	{
		if(clickedFunction!=NULL)
				clickedFunction(this,master);
	}
	void glScrollbar::scrollUp()
	{
		setScrollPosition(scroll_position-10);
	}
	void glScrollbar::scrollDown()
	{
		setScrollPosition(scroll_position+10);
	}

#endif