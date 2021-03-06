//Primary author: Jonathan Bedard
//Confirmed working: 2/2/2015

/*This file contains the OS specific functions needed in opengl*/

/*
WINDOWS ONLY
*/

#ifndef OSGRAPHICS_H
#define OSGRAPHICS_H

#include <stdio.h>
#include <string>
#include <cstring>

#include <stdio.h>
#include <iostream>

#include <winsock2.h>
#include <windows.h>
#include "freeglut.h"
#include "freeglut_ext.h"
#include "freeglut_std.h"
#include "glut.h"

//Constructs the form
static void formBuilder(int argc, char **argv, glForm* hld, void (*dispFunc)(),void (*keyBoard)(unsigned char, int, int),void (*mouse)(int,int,int,int))
{
	glutInit(&argc, argv);                                      // GLUT initialization
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );  // Display Mode
	glutInitWindowSize(hld->getWidth(),hld->getHeight());		// set window size
	glutCreateWindow(hld->getTitle());							// create Window
	glutDisplayFunc(dispFunc);									// register Display Function
	glutIdleFunc( dispFunc );									// register Idle Function
    glutKeyboardFunc( keyBoard );								// register Keyboard Handler
	glutMouseFunc( mouse );										// register Mouse Handler
}
//Resets the form size and name
static void formReset(glForm* x)
{
	glutSetWindowTitle(x->getTitle());
	glutReshapeWindow(x->getWidth(),x->getHeight());
}
//Forces the window to remain the same size
static void checkForceSize(glForm* x)
{
	if(x->getLocked()&&(x->getWidth() != glutGet(GLUT_WINDOW_WIDTH) || x->getHeight() != glutGet(GLUT_WINDOW_HEIGHT)))
	{
		glutReshapeWindow(x->getWidth(),x->getHeight());
	}
	else if(!(x->getLocked()))
	{
		x->setWidth(glutGet(GLUT_WINDOW_WIDTH));
		x->setHeight(glutGet(GLUT_WINDOW_HEIGHT));
	}
}
//Sets the size of a frame
static void setFormSize(glForm* x)
{
	glutReshapeWindow(x->getWidth(),x->getHeight());
}
//The opengl render loop
static void glRenderBegin()
{
	glutMainLoop();
}
//Draws text
static void drawText(float x, float y, char * string, color c, void* font)
{
	int len;
	glColor4f(c.red,c.green,c.blue,c.alpha);

	glRasterPos2i(x,y);
	
	glDisable(GL_TEXTURE);
	for(int i=0, len = strlen(string);i<len;i++)
	{
		glutBitmapCharacter(font, (int) string[i]);
	}
}

#endif