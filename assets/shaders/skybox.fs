#version 330 core

in vec3 uv;

struct DirectionalLight {
    vec3 direction;
    vec3 color;
};

uniform DirectionalLight light;

out vec4 fragColor;

void main() {
    float value = light.direction.y;
    vec4 color_blue =  vec4(0.4, 0.7, 0.8, 1.0);
    vec4 color_red =  vec4(0.9, 0.3, 0.2, 1.0);
    if (value > 0.0) {
    fragColor = mix(color_red, color_blue, value);
    } else {
        fragColor = mix(color_red, vec4(0.0,0.0,0.0,1.0), abs(value));
    }
}
