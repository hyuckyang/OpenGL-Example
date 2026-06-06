#include "Framework_Drags.h"

void Framework_Drags::KeyFuncEvent(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	Framework_Based::KeyFuncEvent(window, key, scancode, action, mode);

	switch (key) 
	{
		case GLFW_KEY_R:
		{
			mat = matDrag = matUpdated = glm::mat4(1.0F); // reset every thing
			break;
		}		
	}
}

void Framework_Drags::MouseButtonEvent(GLFWwindow* window, int button, int action, int mods)
{
	GLdouble x, y;
	switch (action) 
	{
		case GLFW_PRESS:
		{
			mousePressed = GL_TRUE;
			glfwGetCursorPos(window, &x, &y);
			dragStart = glm::vec2((GLfloat)x, (GLfloat)y);
			break;
		}
			
		case GLFW_RELEASE:
		{
			mousePressed = GL_FALSE;
			glfwGetCursorPos(window, &x, &y);
			glm::vec2 dragCur = glm::vec2((GLfloat)x, (GLfloat)y);
			matDrag = CalcTrackball(dragStart, dragCur);
			mat = matDrag * matUpdated;
			matDrag = glm::mat4(1.0F); // reset
			matUpdated = mat; // update to the object matrix
			break;
		}
	}
}

void Framework_Drags::CursorEnterEvent(GLFWwindow* window, int entered)
{
	///
}

void Framework_Drags::CursorPosEvent(GLFWwindow* window, double xPos, double yPos)
{
	if (mousePressed == GL_TRUE)
	{
		glm::vec2 dragCur = glm::vec2((GLfloat)xPos, (GLfloat)yPos);
		matDrag = CalcTrackball(dragStart, dragCur);
		mat = matDrag * matUpdated;
	}
}

glm::vec3 Framework_Drags::CalcUnitVec(const glm::vec2& raw)
{
	glm::vec2 scr;

	scr.x = (raw.x < 0.0F ? 0.0F : (raw.x > WIN_W ? WIN_W : raw.x));
	scr.y = (raw.y < 0.0F ? 0.0F : (raw.y > WIN_H ? WIN_H : raw.y));

	// normal processing
	const GLfloat radius = sqrtf(WIN_W * WIN_W + WIN_H * WIN_H) / 2.0F;
	glm::vec3 v;
	v.x = (scr.x - WIN_W / 2.0F) / radius;
	v.y = (WIN_H / 2.0F - scr.y) / radius;
	v.z = sqrtf(1.0F - v.x * v.x - v.y * v.y);
	return v;
}

glm::mat4 Framework_Drags::CalcTrackball(const glm::vec2& start, const glm::vec2& cur)
{
	glm::vec3 org = CalcUnitVec(start);
	glm::vec3 dst = CalcUnitVec(cur);
	glm::quat q = glm::rotation(org, dst);
	glm::mat4 m = glm::toMat4(q);
	return m;
}
