#version 450 core

in vec2 fragUV;
in vec4 fragClip;

uniform sampler2D reflection;
uniform sampler2D refraction;

out vec4 fragColor;

void main() {
    vec2 ndc = (fragClip.xy / fragClip.w) / 2.0 + 0.5;
    vec2 refractionUV = ndc;
    vec2 reflectionUV = vec2(ndc.x, 1.0 - ndc.y);
    vec4 reflectionColor = texture(reflection, reflectionUV);
    vec4 refractionColor = texture(refraction, refractionUV);
    fragColor = mix(reflectionColor, refractionColor, 0.5); 
}
