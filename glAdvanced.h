//Primary author: Jonathan Bedard
//Confirmed working: 12/8/2014

#pragma once

#ifndef GLADVANCED_H
#define GLADVANCED_H

#define KEY_ESCAPE 27
#define KEY_BACKSPACE 8

#define KEY_UP 101
#define KEY_LEFT 100
#define KEY_DOWN 103
#define KEY_RIGHT 102

#include <string>
#include <list>

using namespace std;

//This is color struct
struct color
{
	float red;
	float green;
	float blue;
	float alpha;
};

//glElements---------------------------------------------------------------------------------------------------

//The gl form element
class glElement
{
protected:
	color col;

	//Click event
	bool clicked;
	bool mouseOver;
	void* master;
	void (*clickedFunction)(glElement*,void*);
	void (*pressedFunction)(glElement*,void*);
	void (*depressedFunction)(glElement*,void*);
	
	//Button Dimensions
	int xPos;
	int yPos;

	int width;
	int height;

public:
	glElement();
	virtual ~glElement();

	//Set Functions
	void setX(int x);
	void setY(int x);
	void setWidth(int x);
	void setHeight(int x);
	void setColor(color c);

	//Get Functions
	int getX();
	int getY();
	int getWidth();
	int getHeight();
	color getColor();
	bool virtual getKeyboardStatus();

	//Listeners
	void setClickEvent(void (*func)(glElement*,void*));
	void setPressedEvent(void (*func)(glElement*,void*));
	void setReleaseEvent(void (*func)(glElement*,void*));
	void virtual setMaster(void* m);
	bool virtual clickListener(int button, int state, int x, int y);
	void virtual keyListener(char key);
	void virtual specialKeyListener(int key);
	
	//Graphics Functions
	void virtual draw();
	void virtual top_draw();
	bool virtual mouseIn(int x, int y);
};
//The gl label
class glLabel:public glElement
{
protected:
	char* text;
	color textColor;

	bool centered;
	void* font;
	int textWidth;
	int textHeight;

	void setTextSize();
public:
	glLabel();
	virtual ~glLabel();

	//Set Functions
	void setText(char* t);
	void setTextColor(color c);
	void setFont(void* x);
	void setCentered(bool x);

	//Get Functions
	char* getText();
	color getTextColor();
	void* getFont();
	bool getCentered();

	//Graphics Functions
	void virtual draw();
};
//The gl button
class glButton:public glLabel
{
protected:
	color clickedColor;
	int h_b;

public:
	glButton();
	virtual ~glButton();

	//Set Functions
	void setClickedColor(color c);
	void setBorderSize(int s);

	//Get Functions
	color getClickedColor();
	int getBorderSize();

	//Graphics Functions
	void virtual draw();
};
//A glButton package for nested lists
class nestedButtons
{
protected:
	bool master;
	bool isClicked;

public:
	//Note that nested buttons clean themselves up
	nestedButtons();
	virtual ~nestedButtons();
	void setMaster(bool x);
	void setClicked(bool x);

	void draw();
	void setLocation();
	bool cascadeClick(int button, int state, int x, int y);
	bool mouseIn(int x, int y);
	void setMasterPointer(void* m);
	bool getIsClicked();
	
	glButton btn;
	list<nestedButtons*>* nested_list;
	bool triggered;
};
//The gl drop down menu
class glFormMenu:public glElement
{
protected:
	list<nestedButtons*>* button_list;
	bool invisible;

public:
	glFormMenu();
	virtual ~glFormMenu();
	void virtual draw();
	void virtual top_draw();
	void force_resize();
	
	//Set Functions
	void setNestedButtons(list<nestedButtons*>* buttons);
	void virtual setMaster(void* m);
	void setClosed();
	bool virtual clickListener(int button, int state, int x, int y);
	bool virtual mouseIn(int x, int y);
	void setHidden(bool x);
};
//The gl textbox element
class glTextbox:
	public glElement
{
protected:
	string text;
	string true_text;

	color textColor;
	color clickedColor;

	bool centered;
	void* font;
	int textHeight;
	int max_chars;

	int cursor_loc;
	int old_curs;
	int curse_blink;
	int setCurseLoc(int x);

	bool display;

	list<char> bad_inputs;
public:
	glTextbox();
	virtual ~glTextbox();

	bool virtual clickListener(int button, int state, int x, int y);

	//Set Functions
	void setText(char* t);
	void setTextColor(color c);
	void setFont(void* x);
	void setMaxChars(int x);
	void setDisplayType(bool x);
	void pushBadInput(char x);
	void setClickedColor(color c);

	//Get Functions
	char* getText();
	color getTextColor();
	void* getFont();
	bool virtual getKeyboardStatus();
	color getClickedColor();

	//Graphics Functions
	void virtual draw();

	//Listeners
	void virtual keyListener(char key);
	void virtual specialKeyListener(int key);
};

//Forms--------------------------------------------------------------------------------------------------------

//This is the generalized form class
class glForm
{
protected:
	int width;
	int height;
	char* title;
	color background;

	bool lockDisplay;
	bool reDisplay;
	void virtual initialize();

	list<glElement*> elmList;
	list<glElement*> clickEvents;
	glElement* focus;

	bool form_return_flag;
	glForm* previous_form;
	glForm* next_form;
	bool is_pop_up;

public:
	//Constructor/Destructor
	glForm(int argv, char ** addr, int wid, int hei, char* ti,
			void (*dispFunc)(),void (*keyBoard)(unsigned char, int, int),void (*mouse)(int,int,int,int),
			void (*mouseMove)(int,int),void (*kU)(unsigned char, int, int),
			void (*spc_down)(int,int,int),void (*spc_up)(int,int,int));
	glForm(glForm* prev);
	virtual ~glForm();

	//Get Functions
	int getWidth();
	int getHeight();
	char* getTitle();
	color getColor();
	bool getLocked();

	//Set Functions
	void setWidth(int x);
	void setHeight(int x);
	void setTitle(char* t);
	void setColor(color c);
	void pushElement(glElement* x);
	void addMouseListener(glElement* x);
	void removeElement(glElement* x);
	void setLocked(bool x);
	void forceSize(int x, int y);
	void setNextForm(glForm* x);
	void setPopUp(glForm* popStr);
	void virtual sendMessage(int x);

	//Graphics Function
	void clear();
	void background_draw();
	void virtual virtual_background_draw();
	void virtual draw();
	void virtual virtual_draw();
	void top_draw();
	void virtual virtual_top_draw();
	void display();
	void drawElements();

	//Update Functions
	void update();
	void virtual virtual_update();
	void virtual return_form();
	void virtual exit_form();

	//Action Functions
	void formResize();
	void virtual virtualFormResize();
	void mouse(int button, int state, int mousePositionX, int mousePositionY);
	void virtual virtualMouse(int button, int state, int mousePositionX, int mousePositionY);
	void keyboard(unsigned char key, int mousePositionX, int mousePositionY);
	void virtual virtualKeyboard(unsigned char key, int mousePositionX, int mousePositionY);
	void mouseMove(int x, int y);
	void virtual virtualMouseMove(int x, int y);
	void keyUp(unsigned char key, int mousePositionX, int mousePositionY);
	void virtual virtualKeyUp(unsigned char key, int mousePositionX, int mousePositionY);
	
	void specialKeyDown(int key, int mousePositionX, int mousePositionY);
	void virtual virtualSpecialKeyDown(int key, int mousePositionX, int mousePositionY);
	void specialKeyUp(int key, int mousePositionX, int mousePositionY);
	void virtual virtualSpecialKeyUp(int key, int mousePositionX, int mousePositionY);
};
//This is the form class  for 3d data
class glForm3d:
	public glForm
{
protected:
	float field_of_view_angle;
	float z_near;
	float z_far;
public:
	//Constructor/Destructor
	glForm3d(int argv, char ** addr, int wid, int hei, char* t,
			float fov,float zn,float zf,
			void (*dispFunc)(),void (*keyBoard)(unsigned char, int, int),void (*mouse)(int,int,int,int),
			void (*mouseMove)(int,int),void (*kU)(unsigned char, int, int),
			void (*spc_down)(int,int,int),void (*spc_up)(int,int,int));
	glForm3d(int wid, int hei, char* ti);
	virtual ~glForm3d();

	//Get Functions
	float getFOV();
	float getZNear();
	float getZFar();

	//Set Functions
	void setFOV(float x);
	void setZNear(float x);
	void setZFar(float x);

	//Graphics Function
	void virtual draw();
	void virtual td_draw();
};
//This is a pop-up
class glPopUp:
	public glForm
{
protected:
	glForm* master;
	glLabel lblStatus;
	int widthDiv;
	int heightDiv;

	color master_background;
public:
	glPopUp(glForm* f, char* str);
	void virtual virtual_background_draw();
};
#endif