#version 450 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 fragUV;
out vec4 fragClip;

void main() {
    fragUV = uv;
    fragClip = projection * view * model * vec4(position, 1.0);
    gl_Position = fragClip;
}
