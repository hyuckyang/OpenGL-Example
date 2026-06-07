#pragma once

#include "../../Framework/Framework_ShaderLoader.h"
#include <vector>

struct FVertex
{
	glm::vec2 pos;
	glm::vec4 color;
};

class FieldOfView2D : public Framework_ShaderLoader
{
public:
	FieldOfView2D();

protected:

	void CreateCircle(float radius, int VertCount);

	bool CheckFOV(glm::vec2 point);

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

	float	  deltaTime = 0.f;
	float	  lastTime = 0.f;

	// 원 
	vector<FVertex> circleData;
	GLuint circleVAO;
	GLuint circleVBO;

	// 시야각 (FOV)
	vector<FVertex> fovData;
	GLuint fovVAO;
	GLuint fovVBO;

	// 감지될 점 
	vector<glm::vec2> basePoints;

	// 감지되어 그려질 점 
	vector<FVertex> pointsData;
	GLuint pointsVAO;
	GLuint pointsVBO;
};


