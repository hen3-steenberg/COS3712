#version 460
#include "glNoise/Common.glsl"
#include "glNoise/Perlin.glsl"

#define Step 0.5f
#define NumCols 1000
#define Offset (-999.0 * Step)

layout(push_constant, std430) uniform model
{
    mat4 transform;
    vec2 position;
};

layout(location = 0) out vec3 fragColor;

vec2 offsets[6] = vec2[](vec2(-Step + Offset, Step + Offset),
                         vec2(Step + Offset, Step + Offset),
                         vec2(-Step + Offset, -Step + Offset),
                         vec2(Step + Offset, Step + Offset),
                         vec2(Step + Offset, -Step + Offset),
                         vec2(-Step + Offset, -Step + Offset));

float
getHeight(vec2 mapPos)
{
    return 5.0 * gln_perlin(mapPos / 20.0);
}

vec3
getVertex(uint Row, uint Col, uint square, vec2 pos)
{
    vec2 xy = vec2(2 * Col * Step, 2 * Row * Step) + pos;
    xy += offsets[square];
    return vec3(xy, getHeight(xy));
}

vec2
getSlope(vec3 pos)
{
    const float delta = 0.1;
    float dx = getHeight(vec2(pos.x + delta, pos.y)) - pos.z;
    float dy = getHeight(vec2(pos.x, pos.y + delta)) - pos.z;
    return vec2(dx, dy) / delta;
}

vec3 colors[6] = vec3[](vec3(1.0, 0.0, 0.0),
                        vec3(0.0, 1.0, 0.0),
                        vec3(0.0, 0.0, 1.0),
                        vec3(1.0, 0.0, 0.0),
                        vec3(0.0, 1.0, 0.0),
                        vec3(0.0, 0.0, 1.0));

void
main()
{
    uint gridIndex = gl_VertexIndex / 6;
    uint squareIndex = gl_VertexIndex % 6;
    uint Col = gridIndex / NumCols;
    uint Row = gridIndex % NumCols;
    vec3 pos = getVertex(Row, Col, squareIndex, position);
    gl_Position = transform * vec4(pos, 1.0);
    vec2 slope = getSlope(pos);
    float mag = distance(slope.x, slope.y);
    mag = clamp(mag, 0.0, 1.0);
    fragColor = 0.8 * (vec3(0.4, 0.6, 0.0) + mag * vec3(0.6, -0.3, 0.1)) + (0.5 * (2.0 - pos.z) * vec3(-0.2, 0.0, 0.2));
}