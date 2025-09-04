#version 460
#include "glNoise/Common.glsl"
#include "glNoise/Perlin.glsl"

#define Step 0.5f
#define NumCols 1000
#define Offset (-999.0 *Step)

layout(push_constant, std430) uniform model {
    mat4 transform;
    vec3 cameraPos;
    vec3 sunDirection;
    vec3 sunColor;
};

layout(location = 0) out vec3 fragColor;

vec2 offsets[6] = vec2[](
    vec2(-Step + Offset, Step + Offset), vec2(Step + Offset, Step + Offset), vec2(-Step + Offset, -Step + Offset),
    vec2(Step + Offset, Step + Offset), vec2(Step + Offset, -Step + Offset), vec2(-Step + Offset, -Step + Offset)
);

float getHeight(vec2 mapPos)
{
    return 5.0 * gln_perlin(mapPos / 20.0);
}

vec3 getVertex(uint Row, uint Col, uint square, vec2 pos)
{
    vec2 xy = vec2(2 * Col * Step, 2 * Row * Step) + pos;
    xy += offsets[square];
    return vec3(xy, getHeight(xy));
}

vec3 getDiffuse(vec3 normal, vec3 lightDirection, vec3 lightColour)
{
    float factor = max(dot(lightDirection, normal), 0);
    return factor * lightColour;
}

vec3 getSpeccular(vec3 normal, vec3 lightDirection, vec3 lightColour, vec3 cameraPos, vec3 pos)
{
    float factor = dot(lightDirection, normal);
    vec3 R = 2 * factor * normal - lightDirection;
    R = normalize(R);
    vec3 inRay = normalize(pos - cameraPos);
    float factor2 = dot(R, inRay);
    return factor2 * lightColour;
}

void main() {
    uint gridIndex = gl_VertexIndex / 6;
    uint squareIndex = gl_VertexIndex % 6;
    uint Col = gridIndex / NumCols;
    uint Row = gridIndex % NumCols;
    vec3 positions[3];
    uint offset = (squareIndex > 2) ? 3 : 0;
    vec2 position = vec2(floor(cameraPos.x), floor(cameraPos.y));
    for(uint idx = 0; idx < 3; ++idx)
    {
        positions[idx] = getVertex(Row, Col, idx + offset, position);
    }
    vec3 pos = positions[squareIndex - offset];
    gl_Position = transform * vec4(pos, 1.0);
    float height = (positions[0].z + positions[1].z + positions[2].z) / 3.0;
    vec3 direction = cross(positions[1] - positions[0], positions[2] - positions[0]);
    vec3 fragNormal = normalize(direction);
    float mag = distance(direction.x, direction.y);
    mag = clamp(mag, 0.0, 1.0);
    vec3 ambient = 0.8 * (vec3(0.4, 0.6, 0.0) + mag * vec3(0.6, -0.3, 0.1)) +  (0.5 * (2.0 - height) * vec3(-0.2, 0.0, 0.2));
    fragColor = 0.3 * ambient +
                0.5 * getDiffuse(fragNormal, sunDirection, sunColor * ambient) +
                0.2 * getSpeccular(fragNormal, sunDirection, sunColor, cameraPos, pos);
}