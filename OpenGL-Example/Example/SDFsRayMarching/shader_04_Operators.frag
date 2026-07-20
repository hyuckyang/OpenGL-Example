#version 330 core

in vec2 TexCoord;
out vec4 FragColor;

uniform vec2 iRes;
uniform float iTime;

float smin(float a, float b, float k)
{
    float h = max(k-abs(a-b), 0.0f)/k;
    return min(a, b) - h*h*h*k*(1.0f/6.0f);
}

float onUnion(float d1, float d2)
{
    return min(d1, d2);
}

float onSmoothUnion(float d1, float d2, float k)
{
    float h = clamp(0.5f + 0.5f * (d2 - d1)/k, 0.0f, 1.0f);
    return mix(d2, d1, h) - k*h*(1.0f-h);
}

float onSubtraction(float d1, float d2)
{
    return max(-d1, d2);
}

float onSmoothSubtraction(float d1, float d2, float k)
{
    float h = clamp(0.5f - 0.5f * (d2 + d1)/k, 0.0f, 1.0f);
    return mix(d2, -d1, h) + k*h*(1.0f-h);
}

float onIntersection(float d1, float d2)
{
    return max(d1, d2);
}

float onSmoothIntersection(float d1, float d2, float k)
{
    float h = clamp(0.5f - 0.5f * (d2 - d1)/k, 0.0f, 1.0f);
    return mix(d2, d1, h) + k*h*(1.0f-h);
}

float sdSphere(vec3 p, float s)
{
    return length(p) - s; 
}

float sbBox(vec3 p, vec3 b)
{
    vec3 q = abs(p) - b;
    return length(max(q,0.0f)) + min(max(q.x, max(q.y, q.z)), 0.0f);
}

float map(vec3 p)
{
    vec3 spherePos = vec3(sin(iTime) * 3.0f, 0.0f, 0.0f);
    float sphere = sdSphere(p - spherePos, 1.f); // SDFs Sphere (원점은 0,0,0)

    float box = sbBox(p, vec3(0.75f));

    return min(sphere, box); 
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

        //col = vec3(i) / 80.f;

        if(d < .001f) break;
        if(t > 100.f) break;
    }

    col = vec3(t * 0.2f);
    
    FragColor = vec4(col, 1.0f);
}