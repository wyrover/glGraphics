//Primary author: Jonathan Bedard
//Confirmed working: 1/31/2015

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
	mkdir(path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}
//Check if a file is a directory
bool gl_is_directory(string file)
{
	struct stat st_buf;
	
	if(stat (file.c_str(), &st_buf))
		return false;
	
	if (S_ISDIR (st_buf.st_mode))
		return true;
	
	return false;
}
//Return a string array of all the elements in a directory
string* gl_list_files(string directory, int* len)
{
	DIR *dir;
	struct dirent *ent;

	if ((dir = opendir (directory.c_str())) != NULL)
	{
		//Count the number of files in the directory
		*len = 0;
		while ((ent = readdir (dir)) != NULL)
		{
			if(ent->d_name[0]!='.')
				*len = *len+1;
		}
		
		//Confirm proper size and create return array
		if(*len<1)
		{
			closedir(dir);
			return NULL;
		}
		string* ret = new string[*len];
		
		//Read all elements into an array
		rewinddir(dir);
		*len = 0;
		while ((ent = readdir (dir)) != NULL)
		{
			if(ent->d_name[0]!='.')
			{
				ret[*len] = directory+"/"+string(ent->d_name);
				*len = *len+1;
			}
		}
		
		closedir(dir);
		return ret;
	}
	//Bad directory, return NULL
	else
	{
		*len = 0;
		return NULL;
	}
}
//Extract the file/directory
string gl_extract_name(string full_path)
{
	int bck = full_path.length()-1;
	string ret = "";
	
	while(full_path.c_str()[bck]!='/' && full_path.c_str()[bck]!='\\')
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
		int num;
		string* files = gl_list_files(path,&num);
		
		if(files != NULL)
		{
			int cnt = 0;
			
			while(cnt<num)
			{
				gl_delete_file(files[cnt]);
				cnt++;
			}
			delete[] files;
		}
	}
	remove((char*)path.c_str());
}
#endif