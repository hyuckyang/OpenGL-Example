#pragma once

#include <string>

using namespace std;

namespace Utils
{
	string getBaseName(const char* progName);
	string getVertName(const char* baseName);
	string getFragName(const char* baseName);
	string loadFile(const char* fileName);
}