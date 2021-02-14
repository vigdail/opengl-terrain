#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;

out VS_OUT {
    vec4 position;
    vec2 uv;
} vs_out;

uniform mat4 model;
uniform mat4 projection;

void main() {
    vs_out.uv = uv;
    vs_out.position = projection * model * vec4(position.xy, 0.0, 1.0);
    gl_Position = vs_out.position;
}
