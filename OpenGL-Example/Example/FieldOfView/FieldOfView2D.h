#pragma once

#include "../../Framework/Framework_ShaderLoader.h"
#include <vector>

class FieldOfView2D : public Framework_ShaderLoader
{
public:
	FieldOfView2D();

protected:

	void CreateCircle(float radius, int VertCount);
	//void CreatePoints();

	bool CheckFOV(glm::vec4 point);

	virtual void InitFunc(GLFWwindow* window) override;
	virtual void DrawFunc(GLFWwindow* window) override;
	virtual void UpdateFunc(GLFWwindow* window) override;
	virtual void KeyFuncEvent(GLFWwindow* window, int key, int scancode, int action, int mode) override;

private:

	float	  radian = 1.f;

	glm::vec3 playerPos;
	float     plyaerPosSpeed = 1.f;
	float	  playerRot = 90.f; // 정면
	float	  playerRotSpeed = 180.f;
	float	  playerFOVAngle = 60.f;
	glm::vec3 playerForward;

	float	  inputForward = 0.f;
	float	  inputRight = 0.f;

	float					 deltaTime = 0.f;
	system_clock::time_point lastTime = system_clock::now(); // 선언 시, 현재 시간 정의

	vector<glm::vec4> vertexCircle;
	vector<glm::vec4> vertexFOV;
	vector<glm::vec4> vertexPoints;
};


