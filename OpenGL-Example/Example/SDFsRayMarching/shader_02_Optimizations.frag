#version 330 core

in vec2 TexCoord;
out vec4 FragColor;

uniform vec2 iRes;
uniform float iTime;

float sdSphere(vec3 p, float s)
{
    return length(p) - s; 
}

float map(vec3 p)
{
    float sphere = sdSphere(p, 1.f); // SDFs Sphere (원점은 0,0,0)

    return sphere; 
}

void main(void) 
{
    vec2 fragCoord = TexCoord * iRes; // pixel
    vec2 uv = (fragCoord * 2.f - iRes.xy) / iRes.y;
    
    // 초기화
    vec3 ro = vec3(0.f, 0.f, -3.f); // origin 
    vec3 rd = normalize(vec3(uv, 1.f)); // direction;  uv -> x , y  좌표계
    vec3 col = vec3(0.f);

    float t = 0.f; // total distance travelled (광선의 이동거리?)

    // RayMarching
    for(int i = 0; i < 80; i++)
    {
        vec3 p = ro + rd * t; // 시간에 따라 지정된 방향으로 p 를 발사
        float d = map(p);
        t += d; // 거리 누적

        col = vec3(i) / 80.f;

        if(d < .001f) break;
        if(t > 100.f) break;
    }

    //col = vec3(t * 0.2f);
    
    FragColor = vec4(col, 1.0f);
}