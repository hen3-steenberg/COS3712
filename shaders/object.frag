#version 450
#include "light/flat.glsl"

layout(push_constant, std430) uniform model {
    layout(offset = 144) vec3 sunDirection;
    vec3 sunColor;
    vec3 base_color;
    float Ka;
    float Kd;
    float Ks;
};


layout(location = 0) in vec3 inNormal;
layout(location = 1) in vec3 inCameraDir;

layout(location = 0) out vec4 outColor;

void main() {
    vec3 color3 = Ka * base_color
                + Kd * getDiffuse(inNormal, sunDirection, sunColor * base_color)
                + Ks * getSpeccular(inNormal, sunDirection, sunColor, inCameraDir);
    outColor = vec4(color3, 1.0);
}