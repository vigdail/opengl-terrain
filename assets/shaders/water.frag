#version 450 core

in vec2 fragUV;
in vec4 fragClip;
in vec4 worldPos;

struct DirectionalLight {
    vec3 direction;
    vec3 color;
    float intensity;
};

layout(std140, binding = 0) uniform Camera {
    vec3 camera_position;
    mat4 view;
    mat4 projection;
};

uniform sampler2D reflection;
uniform sampler2D refraction;
uniform sampler2D dudv;
uniform sampler2D normalmap;
uniform sampler2D depthmap;
uniform float time;
uniform DirectionalLight sun;

uniform float dudv_tiling;
uniform float specular_power;
uniform float reflection_power;
uniform float depth_factor = 2.0;
uniform vec3 color = vec3(0.0, 0.4, 0.6);
uniform float wave_strength = 0.003;
uniform float wave_speed_factor = 250.0f;

uniform float camera_near;
uniform float camera_far;

out vec4 fragColor;

float linearDepth(float depth) {
    return 2.0 * camera_far * camera_near / (camera_far + camera_near - (2.0 * depth - 1.0) * (camera_far - camera_near));
}

float getWaterDepth(vec2 uv) {
    float floorDistance = linearDepth(texture(depthmap, uv).r);
    float waterDistance = linearDepth(gl_FragCoord.z);
    return max(0.0, floorDistance - waterDistance);
}

void main() {
    vec2 ndc = (fragClip.xy / fragClip.w) / 2.0 + 0.5;
    vec2 refractionUV = ndc;
    vec2 reflectionUV = vec2(ndc.x, 1.0 - ndc.y);

    float water_depth = getWaterDepth(refractionUV);
    float water_alpha = clamp(water_depth/depth_factor, 0.0, 1.0);
    float water_alpha_distort = clamp(water_depth/(depth_factor*2), 0.0, 1.0);

    float water_offset = time / wave_speed_factor;

    vec2 distortUV1 = vec2(fragUV.x + water_offset/2, fragUV.y) * dudv_tiling;
    vec2 distortUV2 = vec2(-fragUV.x + water_offset/2, fragUV.y + water_offset) * dudv_tiling;
    vec2 distort1 = (texture(dudv, distortUV1).rg * 2.0 - 1.0) * wave_strength * water_alpha_distort;
    vec2 distort2 = (texture(dudv, distortUV2).rg * 2.0 - 1.0) * wave_strength * water_alpha_distort;

    vec2 distorting = distort1 + distort2;
    distorting.y *= 0.5;
    reflectionUV += distorting;
    refractionUV += distorting;

    refractionUV = clamp(refractionUV, 0.001, 0.999);
    reflectionUV = clamp(reflectionUV, 0.001, 0.999);

    vec4 reflectionColor = texture(reflection, reflectionUV);
    vec4 refractionColor = texture(refraction, refractionUV);

    vec3 viewDir = normalize(camera_position - worldPos.xyz);

    vec4 normalSample = texture(normalmap, distorting);
    vec3 normal = vec3(normalSample.r * 2.0 - 1.0, normalSample.b * 8.0, normalSample.g * 2.0 - 1.0);
    normal = normalize(normal);

    float k = max(dot(viewDir, normal), 0.05);
    float reflectiveFactor = pow(k, reflection_power);
    reflectiveFactor = clamp(reflectiveFactor, 0.0, 1.0);

    vec3 halfwayDir = normalize(sun.direction + viewDir);
    float specular = pow(max(dot(normal, halfwayDir), 0.0), specular_power);
    vec3 specularColor = sun.color * specular * 0.1 * clamp(water_depth, 0.0, 1.0);

    fragColor = mix(reflectionColor, refractionColor, reflectiveFactor);
    fragColor = mix(fragColor, vec4(color, 1.0), 0.2) + vec4(specularColor, 0.0);
    fragColor.a = water_alpha;
}
