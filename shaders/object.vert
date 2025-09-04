#version 450
layout(push_constant, std430) uniform model {
    mat4 view_transform;
    mat4 model_transform;
    vec3 cameraPos;
};

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;

layout(location = 0) out vec3 outNormal;
layout(location = 1) out vec3 outCameraDir;

void main() {
    vec4 pos = model_transform *  vec4(inPosition, 1.0);
    vec3 pos3 = vec3(pos.x, pos.y, pos.z);
    gl_Position = view_transform * pos;
    vec4 normal = transpose(inverse(model_transform)) * vec4(inNormal, 1.0);
    outNormal = vec3(normal.x, normal.y, normal.z);
    outCameraDir = normalize(pos3 - cameraPos);
}