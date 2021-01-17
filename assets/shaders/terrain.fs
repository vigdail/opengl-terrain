#version 330 core

in vec4 fragPos;
in vec3 fragNormal;

out vec4 fragColor;

struct DirectionalLight {
    vec3 direction;
    vec3 color;
};

uniform DirectionalLight light;

void main() {
    vec3 norm = normalize(fragNormal);
    float diff = max(dot(light.direction, norm), 0.0);
    vec3 diffuse = diff * light.color;
    fragColor = vec4(diffuse + vec3(0.05), 1.0f); 
    // fragColor = vec4(vec3(fragNormal.r, 0.0f, fragNormal.b), 1.0f); 
}
