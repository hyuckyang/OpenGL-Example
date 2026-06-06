#include "Utils.h"
#include <fstream> // file i/o
#include <sstream> // string buffer
#include <iostream> 

string Utils::getBaseName(const char* progName)
{
	string path(progName); // Create string, auto memory manage

	size_t slashPos = path.find_last_of("\\/"); // find slash
	if (slashPos != string::npos)
	{
		path = path.substr(slashPos + 1); // end find slash , name override
	}

	size_t dotPos = path.find_last_of('.');
	if (dotPos != string::npos)
	{
		path = path.substr(dotPos); // .exe -> delete end override
	}

	return path;
}

string Utils::getVertName(const char* baseName)
{
	string path(baseName);
	
	return path + ".vert";
}

string Utils::getFragName(const char* baseName)
{
	string path(baseName);

	return path + ".frag";
}

string Utils::loadFile(const char* fileName)
{
	ifstream file(fileName);
	if (!file.is_open())
	{
		cout << "file open error [ " << fileName << " ]" << endl; 
	}

	// string temp buffer;
	stringstream buffer;

	// file read
	buffer << file.rdbuf();

	// string 
	return buffer.str();
}
