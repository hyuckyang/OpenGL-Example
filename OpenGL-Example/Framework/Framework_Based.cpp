#include "Framework_Based.h"
#include <string.h>

GLFWwindow* Framework_Based::CreateGL(const char* fileName)
{
	string baseStr = Utils::getBaseName(fileName);

	glfwInit();
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE); // 이중 버퍼
	// 일단 비활성화
	//glfwWindowHint(GLFW_STENCIL_BITS, 8); // 스탠실 버퍼, (Depth)32bit 중 8bit 할당
	//glfwWindowHint(GLFW_DEPTH_BITS, 14); // 스탠실 버퍼, (Depth)32bit 중 24bit 할당

	GLFWwindow* window = glfwCreateWindow(WIN_W, WIN_H , baseStr.c_str(), NULL, NULL);
	glfwSetWindowPos(window, WIN_X, WIN_Y);
	glfwMakeContextCurrent(window);
	glewInit();

	//GLint depth_bits = 0;
	//GLint stencil_bits = 0;

	//// Depth 와, Stencil 이 제대로 활성화 되었는지 파악 (FBO)
	//glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_DEPTH, GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE, &depth_bits);
	//glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_STENCIL, GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE, &stencil_bits);

	// bit 체크
	//printf("depth = %d bit, stencil = %d bit\n", depth_bits, stencil_bits);

	glfwSetWindowUserPointer(window, this);  
	// Callback Register
	glfwSetWindowRefreshCallback(window, RefreshFuncStatic);
	glfwSetKeyCallback(window, KeyFuncStatic);
	glfwSetCursorEnterCallback(window, CursorEnterFuncStatic);
	glfwSetCursorPosCallback(window, CursorPosFuncStatic);
	glfwSetMouseButtonCallback(window, MouseButtonFuncStatic);
	
	// Callback Debug Register
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
	glDebugMessageCallback(DebugFuncStatic, this);
	

	return window;
}

void Framework_Based::MainFunc(int argc, char* argv[])
{
	GLFWwindow* window = CreateGL(argv[0]);

	
	InitFunc(window);
	while (!glfwWindowShouldClose(window))
	{
		// Update
		UpdateFunc(window);

		// Draw
		DrawFunc(window);
		// Read ( Save Image ... )
		ReadFunc(window);
		// GLFW Action
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// done 
	glfwTerminate();
}

void Framework_Based::RefreshFuncStatic(GLFWwindow* window)
{
	if (Framework_Based* based = static_cast<Framework_Based*>(glfwGetWindowUserPointer(window)))
	{
		based->RefreshFuncEvent(window);
	}
}

void Framework_Based::RefreshFuncEvent(GLFWwindow* window)
{
	// Window Refresh
	DrawFunc(window);
	// GLFW Action -> 
	glfwSwapBuffers(window);
}

void Framework_Based::KeyFuncStatic(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (Framework_Based* based = static_cast<Framework_Based*>(glfwGetWindowUserPointer(window)))
	{
		based->KeyFuncEvent(window, key, scancode, action, mode);
	}
}

void Framework_Based::MouseButtonFuncStatic(GLFWwindow* window, int button, int action, int mods)
{
	if (Framework_Based* based = static_cast<Framework_Based*>(glfwGetWindowUserPointer(window)))
	{
		based->MouseButtonEvent(window, button, action, mods);
	}
}

void Framework_Based::CursorEnterFuncStatic(GLFWwindow* window, int entered)
{
	if (Framework_Based* based = static_cast<Framework_Based*>(glfwGetWindowUserPointer(window)))
	{
		based->CursorEnterEvent(window, entered);
	}
}

void Framework_Based::CursorPosFuncStatic(GLFWwindow* window, double xPos, double yPos)
{
	if (Framework_Based* based = static_cast<Framework_Based*>(glfwGetWindowUserPointer(window)))
	{
		based->CursorPosEvent(window, xPos, yPos);
	}
}

void Framework_Based::KeyFuncEvent(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	switch (key)
	{
		case GLFW_KEY_ESCAPE:
		{
			if (action == GLFW_PRESS)
			{
				glfwSetWindowShouldClose(window, GL_TRUE);
			}
			break;
		}
	}
}

void Framework_Based::DebugFuncStatic(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const GLvoid* userParam)
{
	if (Framework_Based* based = static_cast<Framework_Based*>(const_cast<GLvoid*>(userParam)))
	{
		based->DebugFuncEvent(source, type, id, severity, length, message, userParam);
	}
}

void Framework_Based::DebugFuncEvent(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const GLvoid* userParam)
{
	printf("Type: %#x; Source: %#x; ID: %d; Severity: %#x\n", type, source, id, severity);
	printf("Message: %s\n", message);

	cout << "Type: " << type << " Source: " << source << " ID: " << id << " Severity: " << severity << endl;
	cout << "Message : " << message << endl;
}
