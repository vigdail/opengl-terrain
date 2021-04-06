#version 330 core

// uniform mat4 model;
// uniform mat4 view;
// uniform mat4 projection;

uniform vec3 color;

out vec4 fragColor;

void main() {
    fragColor = vec4(color, 1.0);
}
