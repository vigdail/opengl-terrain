#version 450 core

layout(location = 0) in vec2 te_uv;
out vec4 fragColor;

struct DirectionalLight {
    vec3 direction;
    vec3 color;
    float intensity;
};

uniform vec3 color;
uniform vec3 camera;
uniform DirectionalLight light;

uniform sampler2D normalmap;

vec3 diffuse(vec3 dir, vec3 normal, float intensity) {
    return max(vec3(0.01), dot(dir, normal) * light.color);
}

void main() {
    vec3 normal = normalize(texture(normalmap, te_uv).rgb);
    vec3 diff = diffuse(light.direction, normal, light.intensity);
    fragColor = vec4(diff * color, 1.0);
}
