#version 450 core

in vec2 fragUV;
in vec4 fragClip;
in vec4 worldPos;

struct DirectionalLight {
    vec3 direction;
    vec3 color;
    float intensity;
};

uniform sampler2D reflection;
uniform sampler2D refraction;
uniform sampler2D dudv;
uniform sampler2D normalmap;
uniform sampler2D depthmap;
uniform float time;
uniform vec3 camera_position;
uniform DirectionalLight sun;

const float dudv_tiling = 6.0;

out vec4 fragColor;

void main() {
    vec2 ndc = (fragClip.xy / fragClip.w) / 2.0 + 0.5;
    vec2 refractionUV = ndc;
    vec2 reflectionUV = vec2(ndc.x, 1.0 - ndc.y);

    // @TODO: Read from camera
    float near = 0.1;
    float far = 1000.0;
    float depth = texture(depthmap, refractionUV).r;
    float floorDistance = 2.0 * far * near / (far + near - (2.0 * depth - 1.0) * (far - near)); 
    depth = gl_FragCoord.z;
    float waterDistance = 2.0 * far * near / (far + near - (2.0 * depth - 1.0) * (far - near)); 
    float waterDepth = floorDistance - waterDistance;

    vec2 distUV = texture(dudv, vec2(fragUV.x + time / 500.0), fragUV.y).rg * 0.01;
    distUV = fragUV + vec2(distUV.x, distUV.y + time / 500.0);
    distUV *= dudv_tiling;
    vec2 total_dist = (texture(dudv, distUV).rg * 2.0 - 1.0) * 0.01 * clamp(waterDepth / 5.0, 0.0, 1.0);
    reflectionUV += total_dist;
    refractionUV += total_dist;

    refractionUV = clamp(refractionUV, 0.001, 0.999);
    reflectionUV = clamp(reflectionUV, 0.001, 0.999);

    vec4 reflectionColor = texture(reflection, reflectionUV);
    vec4 refractionColor = texture(refraction, refractionUV);

    vec3 viewDir = normalize(camera_position - worldPos.xyz);

    float k = dot(viewDir, vec3(0.0, 1.0, 0.0));
    float reflectiveFactor = pow(k, 0.1);

    vec4 normalSample = texture(normalmap, distUV);
    vec3 normal = vec3(normalSample.r * 2.0 - 1.0, normalSample.b * 5.0, normalSample.g * 2.0 - 1.0);
    normal = normalize(normal);

    vec3 halfwayDir = normalize(sun.direction + viewDir);
    float specular = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    vec3 specularColor = sun.color * specular * 0.1;

    fragColor = mix(reflectionColor, refractionColor, reflectiveFactor); 
    fragColor = mix(fragColor, vec4(0.0, 0.3, 0.4, 1.0), 0.2) + vec4(specularColor, 0.0);
    fragColor.a = clamp(waterDepth, 0.0, 1.0);
}
