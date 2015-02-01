//Primary author: Jonathan Bedard
//Confirmed working: 1/31/2015

//This is the file which contains the declarations for the OS unique functions

/*
WINDOWS ONLY
*/

#ifndef GLOSFUNCTIONS_H
#define GLOSFUNCTIONS_H

#include <stdio.h>
#include <iostream>
#include <stdint.h>
#include <string>

#define _WINSOCKAPI_
#include <Windows.h>
#include <sys/stat.h>

#define DEFAULT_LOC "C:/"

//Sets the source file location
void glSetSource(char* source_string);
//Gets the source file location
std::string glGetSource();
std::string glGetExecutable();
//Gets the depth of the current directory
int glGetSourceDepth();
//Is a base
bool glIsBase(std::string loc);

//Tests if a folder exists and then creates it
void glTestCreateFolder(std::string n);
//Check if a file is a directory
bool gl_is_directory(std::string file);
//Return a string array of all the elements in a directory
string* gl_list_files(std::string directory, int* len);
//Extract the file/directory
string gl_extract_name(std::string full_path);
//Deletes a file or a directory 
void gl_delete_file(std::string path);

#endif