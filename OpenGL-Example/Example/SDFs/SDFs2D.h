#pragma once

#include "../../Framework/Framework_GridShaders.h"

struct SVertex
{
	glm::vec2 pos;
};

class SDFs2D : public Framework_GridShaders
{

public:

	SDFs2D();

protected:

	virtual void InitFunc(GLFWwindow* window) override;
	virtual void DrawFunc(GLFWwindow* window) override;
	virtual void UpdateFunc(GLFWwindow* window) override;

private:
	
	// 화면 덮을 1개의 plane 
	vector<SVertex> planeData;
	GLuint planeVAO;
	GLuint planeVBO;

	// 시간 누적
	float currentTime;
};

