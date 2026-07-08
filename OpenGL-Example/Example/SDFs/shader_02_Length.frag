#version 330 core

in vec2 TexCoord;
out vec4 FragColor;

uniform vec2 iRes;
uniform float iTime;

void main(void) 
{
    vec2 fragCoord = TexCoord * iRes; // pixel
    //vec2 uv = fragCoord / iRes * 2.0 - 1.0; 
    //uv.x *= iRes.x /iRes.y;
    
    vec2 uv = (fragCoord * 2.0 - iRes.xy) / iRes.y;

    float d = length(uv);
    d -= 0.5;
    

    FragColor = vec4(d, d, d, 1.0);
}