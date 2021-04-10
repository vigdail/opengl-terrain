#version 450 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec2 g_uv[];
out vec2 fs_uv;

uniform mat4 view;
uniform mat4 projection;

void main() {

    for (int i = 0; i < gl_in.length(); ++i) {
        vec4 position = gl_in[i].gl_Position;
        gl_Position = projection * view * position;
        fs_uv = g_uv[i];
        EmitVertex();
    }

    EndPrimitive();

}
