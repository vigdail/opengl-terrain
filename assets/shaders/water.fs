#version 450 core

in vec2 fragUV;
in vec4 fragClip;
in vec3 toCamera;

uniform sampler2D reflection;
uniform sampler2D refraction;
uniform sampler2D dudv;
uniform float time;

const float dudv_tiling = 10.0;

out vec4 fragColor;

void main() {
    vec2 ndc = (fragClip.xy / fragClip.w) / 2.0 + 0.5;
    vec2 refractionUV = ndc;
    vec2 reflectionUV = vec2(ndc.x, 1.0 - ndc.y);

    vec2 dist1 = texture(dudv, fragUV * dudv_tiling + vec2(time / 100.0, time / 75.0)).rg * 2.0 - 1.0;
    vec2 dist2 = texture(dudv, fragUV * dudv_tiling + vec2(time / 50.0)).rg * 2.0 - 1.0;
    vec2 total_dist = dist1 + dist2;
    reflectionUV += total_dist / 120.0;
    refractionUV += total_dist / 120.0;

    refractionUV = clamp(refractionUV, 0.001, 0.999);
    reflectionUV = clamp(reflectionUV, 0.001, 0.999);

    vec4 reflectionColor = texture(reflection, reflectionUV);
    vec4 refractionColor = texture(refraction, refractionUV);

    float k = dot(normalize(toCamera), vec3(0.0, 1.0, 0.0));
    float reflectiveFactor = pow(k, 0.5);

    fragColor = mix(reflectionColor, refractionColor, reflectiveFactor); 
    fragColor = mix(fragColor, vec4(0.0, 0.3, 0.4, 1.0), 0.2);
}
