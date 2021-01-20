#version 330 core

in vec3 uv;

struct DirectionalLight {
    vec3 direction;
    vec3 color;
};

uniform DirectionalLight light;
uniform samplerCube skybox;

out vec4 fragColor;

void main() {
    float value = 0.5 * (1.0 + light.direction.y);
    fragColor = texture(skybox, uv) * value;
}
