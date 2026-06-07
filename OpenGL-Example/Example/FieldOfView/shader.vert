#version 330 core

in vec4 aPos;
in vec4 aColor; 
out vec4 vColor;

void main(void) 
{
	gl_Position = aPos;
	//gl_Position.z *= -1.0f;
	vColor = aColor;
}
