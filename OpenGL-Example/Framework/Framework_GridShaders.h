#pragma once

#include "Framework_Based.h"
#include <vector>
#include <string>

struct ShaderInfo
{
	string vertFile;
	string fragFile;

	/*GLuint vertID = 0;
	GLuint fragID = 0;*/
	GLuint progID = 0;

	bool bCompiled = false;
};

class Framework_GridShaders : public Framework_Based
{

protected:

	virtual void InitFunc(GLFWwindow* window) override;

	vector<ShaderInfo> shaderFiles;
	
private:

	bool CompileShader(ShaderInfo& info);


};

