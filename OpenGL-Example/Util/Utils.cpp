#include "Utils.h"
#include <fstream> // file i/o
#include <sstream> // string buffer
#include <iostream> 
#include <io.h> 

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

vector<Utils::ShaderFiles> Utils::loadShaderFileFromDir(const string& path)
{
	vector<Utils::ShaderFiles> shaderFiles;

	// 검색할 경로 포맷 생성 (예: "Example/SDFs/*")
	string searchPath = path;
	if (searchPath.back() != '/' && searchPath.back() != '\\')
	{
		searchPath += "/";
	}
	string folderPath = searchPath; // 원본 폴더 경로 기억 (파일 이름과 합치기 위함)
	searchPath += "*"; // 해당 폴더 안의 모든 파일 검색 기호

	_finddata_t fd;
	intptr_t handle = _findfirst(searchPath.c_str(), &fd);

	// 폴더를 찾지 못했거나 비어있는 경우
	if (handle == -1)
	{
		cout << "Directory not found or empty: " << path << endl;
		return shaderFiles;
	}

	int result = 0;
	// 폴더 내의 파일들을 하나씩 순회
	while (result != -1)
	{
		// 디렉토리(폴더)가 아닌 일반 파일인 경우만 처리
		if ((fd.attrib & _A_SUBDIR) == 0)
		{
			string fileName = fd.name;

			// "shader_"로 시작하고 ".vert"로 끝나는 파일 찾기
			if (fileName.find("shader_") == 0 &&
				fileName.length() >= 5 &&
				fileName.substr(fileName.length() - 5) == ".vert")
			{
				string vertPath = folderPath + fileName;

				// .vert 경로를 바탕으로 .frag 경로 생성
				string fragPath = vertPath;
				fragPath.replace(fragPath.length() - 5, 5, ".frag");

				// 짝이 되는 .frag 파일이 실제 존재하는지 열어봐서 확인
				ifstream f(fragPath.c_str());
				if (f.good())
				{
					Utils::ShaderFiles files;
					files.vertFile = vertPath;
					files.fragFile = fragPath;

					shaderFiles.push_back(files);
				}
			}
		}

		// 다음 파일 찾기
		result = _findnext(handle, &fd);
	}

	// 탐색 핸들 닫기 (메모리 누수 방지)
	_findclose(handle);

	return shaderFiles;
}
