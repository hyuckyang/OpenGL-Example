#pragma once

#include "../../Framework/Framework_ShaderLoader.h"
#include <vector>

struct Vertex 
{
	glm::vec2 pos;   // 위치 데이터 (X, Y)
	glm::vec4 color; // 색상 데이터 (R, G, B, A)
};

struct Rect
{
	float xMin, xMax, yMin, yMax;

	Rect() : xMin(0), xMax(0), yMin(0), yMax(0) {}

	Rect(glm::vec2 center, glm::vec2 halfSize) 
	{
		xMin = center.x - halfSize.x; xMax = center.x + halfSize.x;
		yMin = center.y - halfSize.y; yMax = center.y + halfSize.y;
	}

	bool IsOverlap(const Rect& other) const
	{
		return	this->xMax >= other.xMin && this->xMin <= other.xMax &&
				this->yMax >= other.yMin && this->yMin <= other.yMax;
	}
};

// 장애물 정의
struct Obstacle : public Rect
{
	glm::vec4 color;

	Obstacle(glm::vec2 center, glm::vec2 halfSize) : Rect(center, halfSize) 
	{
		color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f); //
	}
};

// 발사체 정의
struct Projectile : public Rect 
{
	glm::vec2 velocity;

	Projectile(glm::vec2 center, glm::vec2 halfSize) : Rect(center, halfSize) 
	{
		velocity = glm::vec2(0.0f, 0.0f); // 
	}

	// 포탄이 날아갈 때마다 발사체 충돌 Rect 업데이트
	void UpdatePosition(glm::vec2 newCenter, glm::vec2 halfSize) 
	{
		xMin = newCenter.x - halfSize.x; xMax = newCenter.x + halfSize.x;
		yMin = newCenter.y - halfSize.y; yMax = newCenter.y + halfSize.y;
	}
};

// 시뮬레이션 계산 결과
struct PredictionResult 
{
	glm::vec2 hitPoint; // 충돌 위치
	vector<glm::vec2> path; // 궤적
	bool hitObstacle; // 객체와 충돌 되었는지 
};



class Parabola2D : public Framework_ShaderLoader
{

public:

	Parabola2D();

protected:

	virtual void InitFunc(GLFWwindow* window) override;
	virtual void DrawFunc(GLFWwindow* window) override;
	virtual void UpdateFunc(GLFWwindow* window) override;
	virtual void KeyFuncEvent(GLFWwindow* window, int key, int scancode, int action, int mode) override;

private:

	glm::vec2 playerPos;
	float angle;
	float power;
	float gravity;
	float groundY;

	PredictionResult prediction; // 시뮬

	// 바닥
	GLuint groundVAO;
	GLuint groundVBO;
	vector<Vertex> groundData;

	// 장애물
	vector<Obstacle> obstacles;
	vector<Vertex> obsData;
	GLuint obsVAO, obsVBO;

	// 궤적
	GLuint trajVAO, trajVBO;
	vector<Vertex> trajData;
	

private:

	// 궤적 계산 
	PredictionResult CalcTrajectory(glm::vec2 startPos, float angle, float power, float gravity, float groundY, const vector<Obstacle>& obstacles);

	
};

