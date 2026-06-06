#include "Framework_ShaderLoader.h"

void Framework_ShaderLoader::InitFunc(GLFWwindow* window)
{
	// 
	if (vertShaderFile == "" || fragShaderFile == "")
	{
		cout << "error : shader file path is missing" << endl;
		return;
	}

	string vertSource = Utils::loadFile(vertShaderFile); // 쉐이더 파일을 string 으로 받아옴
	const char* vert = vertSource.c_str();

	string fragSource = Utils::loadFile(fragShaderFile); // 쉐이더 파일을 string 으로 받아옴
	const char* frag = fragSource.c_str();

	// Vertex
	vertID = glCreateShader(GL_VERTEX_SHADER); // vertex 쉐이더 생성 및 ID 할당
	glShaderSource(vertID, 1, &vert, NULL);
	glCompileShader(vertID); // Compile .OBJ


	// Fragment
	fragID = glCreateShader(GL_FRAGMENT_SHADER); // fragment 쉐이더 생성 및 ID 할당
	glShaderSource(fragID, 1, &frag, NULL);
	glCompileShader(fragID); // Compile .OBJ

	// prog : Program
	progID = glCreateProgram();
	glAttachShader(progID, vertID);
	glAttachShader(progID, fragID);
	glLinkProgram(progID); // . link to get .EXE

	// execute
	glUseProgram(progID);
}
