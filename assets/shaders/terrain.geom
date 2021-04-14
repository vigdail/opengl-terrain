#version 450 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec2 g_uv[3];
out vec2 fs_uv;

layout(std140, binding = 0) uniform Camera {
    vec3 camera_position;
    mat4 view;
    mat4 projection;
};

uniform vec4 clip_plane;

void main() {

    for (int i = 0; i < gl_in.length(); ++i) {
        vec4 position = gl_in[i].gl_Position;
        gl_ClipDistance[0] = dot(position, clip_plane);
        gl_Position = projection * view * position;
        fs_uv = g_uv[i];
        EmitVertex();
    }

    EndPrimitive();
}
