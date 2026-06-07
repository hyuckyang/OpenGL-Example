#version 330 core

in vec4 aPos;
in vec4 aColor; 

out vec4 vColor;

uniform mat4 uMat; // 직교 투영 행렬


void main(void) 
{
	gl_Position = aPos;
	//gl_Position.z *= -1.0f;
	vColor = aColor;
}
