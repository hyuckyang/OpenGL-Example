#pragma once

#include "Framework_ShaderLoader.h"

class Framework_Drags : public Framework_ShaderLoader
{
protected:

	virtual void KeyFuncEvent(GLFWwindow* window, int key, int scancode, int action, int mode) override;
	virtual void MouseButtonEvent(GLFWwindow* window, int button, int action, int mods) override;
	virtual void CursorEnterEvent(GLFWwindow* window, int entered) override;
	virtual void CursorPosEvent(GLFWwindow* window, double xPos, double yPos) override;

protected:

	glm::vec3 CalcUnitVec(const glm::vec2& raw);
	glm::mat4 CalcTrackball(const glm::vec2& start, const glm::vec2& cur);

	int mousePressed = GL_FALSE;
	glm::vec2 dragStart; // mouse dragging start point

	//Drag Matrix
	glm::mat4 mat = glm::mat4(1.0F); // mat = matDrag * matModel
	glm::mat4 matDrag = glm::mat4(1.0F);
	glm::mat4 matUpdated = glm::mat4(1.0F);
};

