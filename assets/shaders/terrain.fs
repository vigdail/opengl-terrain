#version 330 core

in vec4 fragPos;
in vec3 fragNormal;

out vec4 fragColor;

void main() {
    fragColor = vec4(fragNormal, 1.0f); // vec4(0.2f, 0.8f, 0.3f, 1.0f);
}
