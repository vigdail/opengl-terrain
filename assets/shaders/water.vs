#version 450 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 camera_position;

out vec2 fragUV;
out vec4 fragClip;
out vec4 worldPos;

void main() {
    fragUV = uv;
    worldPos = model * vec4(position, 1.0);
    fragClip = projection * view * worldPos;
    gl_Position = fragClip;
}
