#pragma once

#include <string>
#include <vector>

using namespace std;

namespace Utils
{
	string getBaseName(const char* progName);
	string getVertName(const char* baseName);
	string getFragName(const char* baseName);
	string loadFile(const char* fileName);

	//
	struct ShaderFiles
	{
		string vertFile;
		string fragFile;
	};
	
	vector<ShaderFiles> loadShaderFileFromDir(const string& path); // Ai Code

}