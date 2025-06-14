#version 450

#define Step 0.1f

layout(push_constant, std430) uniform model {
    vec2 position;
};

layout(location = 0) out vec3 fragColor;

void main() {
    gl_Position = vec4(positions[gl_VertexIndex], 0.0, 1.0);
    fragColor = vec3(0.0, 1.0, 0.0);
}