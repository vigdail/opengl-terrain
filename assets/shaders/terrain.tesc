#version 450 core

layout(vertices = 16) out;

layout(location = 0) in vec2 tc_uv[];
layout(location = 0) out vec2 te_uv[];

layout(std140, binding = 0) uniform Camera {
    vec3 camera_position;
    mat4 view;
    mat4 projection;
};
uniform float tessellation_factor;
uniform float tessellation_slope;
uniform float tessellation_shift;

const int AB = 2;
const int BC = 3;
const int CD = 0;
const int DA = 1;

float lodFactor(float dist) {
    float tessellation_level = max(0.0, tessellation_factor/pow(dist, tessellation_slope) + tessellation_shift);

    return tessellation_level;
}

void main() {
    if (gl_InvocationID == 0){
        vec3 ab_mid = vec3(gl_in[0].gl_Position + gl_in[3].gl_Position)/2.0;
        vec3 bc_mid = vec3(gl_in[3].gl_Position + gl_in[15].gl_Position)/2.0;
        vec3 cd_mid = vec3(gl_in[15].gl_Position + gl_in[12].gl_Position)/2.0;
        vec3 da_mid = vec3(gl_in[12].gl_Position + gl_in[0].gl_Position)/2.0;

        float distance_AB = distance(ab_mid, camera_position);
        float distance_BC = distance(bc_mid, camera_position);
        float distance_CD = distance(cd_mid, camera_position);
        float distance_DA = distance(da_mid, camera_position);

        gl_TessLevelOuter[AB] = mix(1, gl_MaxTessGenLevel, lodFactor(distance_AB));
        gl_TessLevelOuter[BC] = mix(1, gl_MaxTessGenLevel, lodFactor(distance_BC));
        gl_TessLevelOuter[CD] = mix(1, gl_MaxTessGenLevel, lodFactor(distance_CD));
        gl_TessLevelOuter[DA] = mix(1, gl_MaxTessGenLevel, lodFactor(distance_DA));

        gl_TessLevelInner[0] = (gl_TessLevelOuter[BC] + gl_TessLevelOuter[DA]) / 4;
        gl_TessLevelInner[1] = (gl_TessLevelOuter[AB] + gl_TessLevelOuter[CD]) / 4;
    }
    te_uv[gl_InvocationID] = tc_uv[gl_InvocationID];
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}
