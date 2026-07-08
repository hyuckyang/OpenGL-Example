#include "Framework_GridShaders.h"

void Framework_GridShaders::InitFunc(GLFWwindow* window)
{
	for (auto& info : shaderFiles)
	{
		info.bCompiled = CompileShader(info);
	}
}

bool Framework_GridShaders::CompileShader(ShaderInfo& info)
{
	if (info.vertFile == "" || info.fragFile == "")
	{
		cout << "error : shader file path is missing" << endl;
		return false;
	}

	string vertSource = Utils::loadFile(info.vertFile.c_str()); // 쉐이더 파일을 string 으로 받아옴
	const char* vert = vertSource.c_str();

	string fragSource = Utils::loadFile(info.fragFile.c_str()); // 쉐이더 파일을 string 으로 받아옴
	const char* frag = fragSource.c_str();

	// Vertex
	GLuint vertID = glCreateShader(GL_VERTEX_SHADER); // vertex 쉐이더 생성 및 ID 할당
	glShaderSource(vertID, 1, &vert, NULL);
	glCompileShader(vertID); // Compile .OBJ


	// Fragment
	GLuint fragID = glCreateShader(GL_FRAGMENT_SHADER); // fragment 쉐이더 생성 및 ID 할당
	glShaderSource(fragID, 1, &frag, NULL);
	glCompileShader(fragID); // Compile .OBJ

	// prog : Program
	info.progID = glCreateProgram();
	glAttachShader(info.progID, vertID);
	glAttachShader(info.progID, fragID);
	glLinkProgram(info.progID); // . link to get .EXE

	// execute
	glUseProgram(info.progID);

	// 안전하게 삭제하자
	glDeleteShader(vertID);
	glDeleteShader(fragID);

	return true;
}
