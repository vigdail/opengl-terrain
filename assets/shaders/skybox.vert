#version 450 core

layout (location = 0) in vec3 position;

layout(std140, binding = 0) uniform Camera {
    vec3 camera_position;
    mat4 view;
    mat4 projection;
};

out vec3 fragPos;

void main() {
    fragPos = position;
    // @TODO: get this from camera UBO
    mat4 v = mat4(mat3(view));
    vec4 pos = projection * v * vec4(position, 1.0);
    gl_Position = pos.xyww;
}
