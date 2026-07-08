#version 330 core

in vec2 TexCoord;
out vec4 FragColor;

uniform vec2 iRes;
uniform float iTime;

void main(void) 
{
    vec2 fragCoord = TexCoord * iRes; // pixel
    vec2 uv = fragCoord / iRes; 
    
    //uv = uv - 0.5; // Center UVs

    FragColor = vec4(uv.x, uv.y, 0.0, 1.0);
}