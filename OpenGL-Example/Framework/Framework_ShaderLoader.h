#pragma once

#include "Framework_Based.h"

class Framework_ShaderLoader : public Framework_Based
{

protected:

	virtual void InitFunc(GLFWwindow* window) override;

	const char* vertShaderFile = "";
	const char* fragShaderFile = "";

	GLuint vertID = 0;
	GLuint fragID = 0;
	GLuint progID = 0;
};

