#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 fragPos;
out vec3 fragNormal;

void main() {
    fragPos = projection * view * model * vec4(position, 1.0);
    fragNormal = normal;
    gl_Position = fragPos;
}
