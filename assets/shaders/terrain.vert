#version 450 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 uv;

layout (location = 0) out vec2 vs_uv;

uniform mat4 local_matrix;
uniform mat4 world_matrix;
uniform vec3 camera;
uniform vec4 clipPlane;

uniform sampler2D heightmap;
uniform float scale_y;

uniform int lod;
uniform vec2 index;
uniform float gap;
uniform vec2 location;
uniform int lod_morph_area[8];

float morph_lat(vec2 pos) {
    vec2 frac = pos - location;

    if (index == vec2(0, 0)){
        float morph = frac.x - frac.y;
        if (morph > 0){
            return morph;
        }
    }
    if (index == vec2(1, 0)){
        float morph = gap - frac.x - frac.y;
        if (morph > 0){
            return morph;
        }
    }
    if (index == vec2(0, 1)){
        float morph = frac.x + frac.y - gap;
        if (morph > 0){
            return -morph;
        }
    }
    if (index == vec2(1, 1)){
        float morph = frac.y - frac.x;
        if (morph > 0){
            return -morph;
        }
    }
    return 0;
}

float morph_lon(vec2 pos) {
    vec2 frac = pos - location;

    if (index == vec2(0, 0)){
        float morph = frac.y - frac.x;
        if (morph > 0) {
            return -morph;
        }
    }
    if (index == vec2(1, 0)){
        float morph = frac.y - (gap - frac.x);
        if (morph > 0) {
            return morph;
        }
    }
    if (index == vec2(0, 1)){
        float morph = gap - frac.y - frac.x;
        if (morph > 0) {
            return -morph;
        }
    }
    if (index == vec2(1, 1)){
        float morph = frac.x - frac.y;
        if (morph > 0) {
            return morph;
        }
    }
    return 0;
}

vec2 morph(vec2 pos, float height, int morph_area) {
    vec2 morphing = vec2(0.0);

    vec2 fixPointLat = vec2(0.0);
    vec2 fixPointLon = vec2(0.0);

    if (index == vec2(0, 0)) {
        fixPointLat = location + vec2(gap, 0);
        fixPointLon = location + vec2(0, gap);
    } else if (index == vec2(1, 0)) {
        fixPointLat = location;
        fixPointLon = location + vec2(gap, gap);
    } else if (index == vec2(0, 1)) {
        fixPointLat = location + vec2(gap, gap);
        fixPointLon = location;
    } else if (index == vec2(1, 1)) {
        fixPointLat = location + vec2(0, gap);
        fixPointLon = location + vec2(gap, 0);
    }

    float distLat = length(camera - (world_matrix * vec4(fixPointLat.x, height, fixPointLat.y, 1.0)).xyz);
    float distLon = length(camera - (world_matrix * vec4(fixPointLon.x, height, fixPointLon.y, 1)).xyz);

    if (distLat > morph_area) {
        morphing.x += morph_lat(pos);
    }
    if (distLon > morph_area) {
        morphing.y += morph_lon(pos);
    }

    return morphing;
}

void main() {
    vec2 local_position = (local_matrix * vec4(position.x, 0, position.y, 1.0)).xz;
    float height = texture(heightmap, local_position).r;
    if (lod > 0) {
        local_position += morph(local_position, height, lod_morph_area[lod-1]);
    }

    vs_uv = local_position;
    height = texture(heightmap, vs_uv).r;

    vec4 world_pos = world_matrix * vec4(local_position.x, height, local_position.y, 1.0);

    gl_ClipDistance[0] = dot(world_pos, clipPlane);

    gl_Position = world_pos;
}