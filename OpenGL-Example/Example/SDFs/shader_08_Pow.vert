#version 330 core

in vec2 aPos;
out vec2 TexCoord;

void main(void) 
{
	gl_Position = vec4(aPos, 0.0, 1.0);
    TexCoord = (aPos + vec2(1.0)) / 2.0; // -1 ~ 1 -> 0 ~ 1;

}
