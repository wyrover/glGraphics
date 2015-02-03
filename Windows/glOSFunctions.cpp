//Primary author: Jonathan Bedard
//Confirmed working: 1/23/2015

//This is the file which contains the declarations for the OS unique functions

/*
WINDOWS ONLY
*/

#ifndef GLOSFUNCTIONS_CPP
#define GLOSFUNCTIONS_CPP

using namespace std;

#include "glOSFunctions.h"

//Sets the source file location
static string glSourceString;
static string glExecutableName;
static int depth;

void glSetSource(char* source_string)
{
	glSourceString = string(source_string);
	glExecutableName = gl_extract_name(glSourceString);
	glSourceString = glSourceString.substr(0,glSourceString.length()-glExecutableName.length()-1);

	depth = 0;
	int cnt = 0;
	while(cnt<glSourceString.length())
	{
		if(glSourceString.at(cnt) == '/' || glSourceString.at(cnt) == '\\')
			depth++;
		cnt++;
	}
}
//Gets the source file location
string glGetSource()
{
	return glSourceString;
}
//Gets the source file executable name
string glGetExecutable()
{
	return glExecutableName;
}
//Gets the depth of the current directory
int glGetSourceDepth()
{
	return depth;
}
//Is a base
bool glIsBase(string loc)
{
	return (loc.length() == 2 && loc.at(1) == ':');
}

//Tests if a folder exists and then creates it
void glTestCreateFolder(std::string n)
{
	const char* path = n.c_str();
	CreateDirectory(path ,NULL);
}
//Check if a file is a directory
bool gl_is_directory(string file)
{
	struct stat s;
	if(stat(file.c_str(),&s)==0)
	{
		if(s.st_mode & S_IFDIR)
			return true;
	}
	return false;
}
//Return a string array of all the elements in a directory
string* gl_list_files(string directory, int* len)
{
	*len = 0;

	//First check if the target is even a directory
	if(!gl_is_directory(directory))
		return NULL;

	WIN32_FIND_DATA fdFile;
    HANDLE hFind = NULL;

    char sPath[2048];

    //Build screen value
    sprintf_s(sPath, "%s\\*.*", directory.c_str());

    if((hFind = FindFirstFile(sPath, &fdFile)) == INVALID_HANDLE_VALUE)
        return NULL;
    do
    {
        if(strcmp(fdFile.cFileName, ".") != 0
                && strcmp(fdFile.cFileName, "..") != 0)
            *len = (*len+1);
    }
    while(FindNextFile(hFind, &fdFile));
    FindClose(hFind);

	string* ret = new string[*len];
	int cnt = 0;
	if((hFind = FindFirstFile(sPath, &fdFile)) == INVALID_HANDLE_VALUE)
        return NULL;
    do
    {
        if(strcmp(fdFile.cFileName, ".") != 0
                && strcmp(fdFile.cFileName, "..") != 0)
		{
			ret[cnt] = directory+"/"+string(fdFile.cFileName);
			cnt++;
		}
    }
    while(FindNextFile(hFind, &fdFile) && cnt< (*len));
    FindClose(hFind);

	return ret;
}
//Extract the file/directory
string gl_extract_name(string full_path)
{
	int bck = full_path.length()-1;
	string ret = "";
	while(full_path.c_str()[bck]!='/'&&full_path.c_str()[bck]!='\\'&&bck>=0)
	{
		ret = full_path.c_str()[bck]+ret;
		bck--;
	}
	return ret;
}
//Deletes a file or a directory 
void gl_delete_file(string path)
{
	if(gl_is_directory(path))
	{
		int cnt = 0;
		int max = 0;
		string* arr= gl_list_files(path, &max);
		while(arr != NULL && cnt<max)
		{
			gl_delete_file(arr[cnt]);
			cnt++;
		}

		RemoveDirectory(path.c_str());
	}
	remove(path.c_str());
}

#endif