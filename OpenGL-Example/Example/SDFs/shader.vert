#version 330 core

in vec2 aPos;

void main(void) 
{
	// 화면만 덮도록 설정
	gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
}
