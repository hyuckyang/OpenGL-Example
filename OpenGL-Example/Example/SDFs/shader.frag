#version 330 core

in vec4 vColor;
out vec4 FragColor; 

uniform vec2 iResolution;
uniform float iTime;

vec3 palette(in float t, in vec3 a, in vec3 b, in vec3 c, in vec3 d) 
{
    return a + b * cos(6.28318 * (c * t + d));
}

void main(void) 
{
    vec2 uv = (gl_FragCoord.xy * 2.0 - iResolution.xy) / iResolution.y;
    vec2 uv0 = uv;
    vec3 finalColor = vec3(0.0);

    
    for (float i = 0.0; i < 4.0; i++) 
    {
        uv = fract(uv * 1.5) - 0.5;

        float d = length(uv) * exp(-length(uv0));

        vec3 col = palette(length(uv0) + i * 0.4 + iTime * 0.4,
                           vec3(0.5, 0.5, 0.5),
                           vec3(0.5, 0.5, 0.5),
                           vec3(1.0, 1.0, 1.0),
                           vec3(0.263, 0.416, 0.557));

        d = sin(d * 8.0 + iTime) / 8.0;
        d = abs(d);
        d = pow(0.01 / d, 1.2);

        finalColor += col * d;
    }

    FragColor = vec4(finalColor, 1.0);
}