#version 330 core

in vec4 fragPos;
in vec2 fragUV;

out vec4 fragColor;

struct DirectionalLight {
    vec3 direction;
    vec3 color;
};

uniform DirectionalLight light;
uniform vec3 color;

uniform sampler2D normalmap;

void main() {
    vec3 norm = texture(normalmap, fragUV).xyz;
    float diff = max(dot(light.direction, norm), 0.0);
    vec3 diffuse = diff * light.color * color;
    vec3 ambient = vec3(0.05);
    fragColor = vec4(diffuse + ambient, 1.0f); 
}
