#pragma once

#pragma region GLM Header

#ifndef __cplusplus
#error This file works only with C++
#endif

#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp> // quaternoion
#include <glm/gtx/string_cast.hpp> // for glm::to_string()
#include <glm/gtc/type_ptr.hpp> // for glm::value_ptr( )

#pragma endregion

#pragma region GLEW/GLFW Header

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glfw3.lib")

#pragma endregion

#define _USE_MATH_DEFINES
#include <math.h>
#include <chrono>
#include <iostream>
#include <algorithm>
using namespace std;
using namespace std::chrono;


#include "Framework.h"
#include "../Util/Utils.h"

class Framework_Based : public Framework
{
public:
	virtual void MainFunc(int argc, char* argv[]) override;

protected:

	unsigned int WIN_W = 500;
	unsigned int WIN_H = 500;
	unsigned int WIN_X = 100;
	unsigned int WIN_Y = 100;

	virtual GLFWwindow* CreateGL(const char* fileName);
	virtual void InitFunc(GLFWwindow* window) {};
	virtual void DrawFunc(GLFWwindow* window) {};
	virtual void UpdateFunc(GLFWwindow* window) {};
	virtual void ReadFunc(GLFWwindow* window) {};
	
	virtual void RefreshFuncEvent(GLFWwindow* window);
	
	virtual void DebugFuncEvent(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const GLvoid* userParam);


#pragma region Input Func
	virtual void KeyFuncEvent(GLFWwindow* window, int key, int scancode, int action, int mode);
	virtual void MouseButtonEvent(GLFWwindow* window, int button, int action, int mods) {};
	virtual void CursorEnterEvent(GLFWwindow* window, int entered) {}
	virtual void CursorPosEvent(GLFWwindow* window, double xPos, double yPos) {}
#pragma endregion


private:
	
	static void RefreshFuncStatic(GLFWwindow* window);
	static void APIENTRY DebugFuncStatic(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const GLvoid* userParam);

	static void KeyFuncStatic(GLFWwindow* window, int key, int scancode, int action, int mode);
	static void MouseButtonFuncStatic(GLFWwindow* window, int button, int action, int mods);
	static void CursorEnterFuncStatic(GLFWwindow* window, int entered);
	static void CursorPosFuncStatic(GLFWwindow* window, double xPos, double yPos);
};

