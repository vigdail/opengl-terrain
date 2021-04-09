#version 450 core

layout(quads, fractional_odd_spacing, cw) in;

layout(location = 0) in vec2 tc_uv[];
layout(location = 0) out vec2 te_uv;

uniform mat4 view;
uniform mat4 projection;
uniform sampler2D heightmap;
uniform float scale_y;

void main() {
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    // world position
    vec4 position =
    ((1 - u) * (1 - v) * gl_in[12].gl_Position +
    u * (1 - v) * gl_in[0].gl_Position +
    u * v * gl_in[3].gl_Position +
    (1 - u) * v * gl_in[15].gl_Position);

    vec2 uv =
    ((1 - u) * (1 - v) * tc_uv[12] +
    u * (1 - v) * tc_uv[0] +
    u * v * tc_uv[3] +
    (1 - u) * v * tc_uv[15]);

    float height = texture(heightmap, uv).r;
    height *= scale_y;
    position.y = height;

    te_uv = uv;
    gl_Position = projection * view * position;
}
