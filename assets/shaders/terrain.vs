#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 fragPos;

void main() {
    fragPos = projection * view * model * vec4(position, 1.0);
    gl_Position = fragPos;
    
}
