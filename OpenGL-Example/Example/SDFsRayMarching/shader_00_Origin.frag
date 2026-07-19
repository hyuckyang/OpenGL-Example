#version 330 core

in vec2 TexCoord;
out vec4 FragColor;

uniform vec2 iRes;
uniform float iTime;

void main(void) 
{
    vec2 fragCoord = TexCoord * iRes; // pixel
    vec2 uv = (fragCoord * 2.f - iRes.xy) / iRes.y;
    
    // 초기화
    vec3 ro = vec3(0.f, 0.f, -3.f); // origin 
    vec3 rd = normalize(vec3(uv, 1.f)); // direction;  uv -> x , y  좌표계

    float t = 0.f; // total distance travelled (광선의 이동거리?)

    FragColor = vec4(uv, 0.0, 1.0);
}