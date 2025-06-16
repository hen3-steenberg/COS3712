#version 460
#include "glNoise/Common.glsl"
#include "glNoise/Perlin.glsl"

#define Step 0.5f
#define NumCols 1000
#define Offset (-999.0 *Step)

layout(push_constant, std430) uniform model {
    mat4 transform;
    vec2 position;
};

layout(location = 0) out vec3 fragColor;

vec2 offsets[6] = vec2[](
    vec2(-Step + Offset, Step + Offset), vec2(Step + Offset, Step + Offset), vec2(-Step + Offset, -Step + Offset),
    vec2(Step + Offset, Step + Offset), vec2(Step + Offset, -Step + Offset), vec2(-Step + Offset, -Step + Offset)
);

vec3 getVertex(uint Row, uint Col, uint square, vec2 pos)
{
    vec2 xy = vec2(2 * Col * Step, 2 * Row * Step) + pos;
    xy += offsets[square];
    return vec3(xy, 5.0 * gln_perlin(xy / 20.0));
}

vec3 colors[6] = vec3[](
    vec3(1.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0), vec3(0.0, 0.0, 1.0),
    vec3(1.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0), vec3(0.0, 0.0, 1.0)
);

void main() {
    uint gridIndex = gl_VertexIndex / 6;
    uint squareIndex = gl_VertexIndex % 6;
    uint Col = gridIndex % NumCols;
    uint Row = gridIndex / NumCols;
    gl_Position = transform * vec4(getVertex(Row, Col, squareIndex, position), 1.0);
    fragColor = colors[squareIndex];
}