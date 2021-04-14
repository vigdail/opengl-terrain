#version 450 core

layout (location = 0) in vec2 position;

layout (location = 0) out vec2 tc_uv;

layout(std140, binding = 0) uniform Camera {
    vec3 camera_position;
    mat4 view;
    mat4 projection;
};

uniform mat4 local_matrix;
uniform mat4 world_matrix;
uniform sampler2D heightmap;
uniform float scale_y;

uniform int lod;
uniform vec2 index;
uniform float gap;
uniform vec2 location;
uniform int lod_morph_area[8];

float morphLat(vec2 pos) {
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

float morphLon(vec2 pos) {
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

vec2 morph(vec2 pos, int morph_area) {
    vec2 morphing = vec2(0.0);

    int i = int(index.x);
    int j = int(index.y);
    vec2 fix_point_lat = location + vec2(1 - i, j) * gap;
    vec2 fix_point_lon = location + vec2(i, 1 - j) * gap;

    float planar_factor = clamp(camera_position.y / scale_y, -1, 1);

    float dist_lat = length(camera_position - (world_matrix * vec4(fix_point_lat.x, planar_factor, fix_point_lat.y, 1.0)).xyz);
    float dist_lon = length(camera_position - (world_matrix * vec4(fix_point_lon.x, planar_factor, fix_point_lon.y, 1.0)).xyz);

    if (dist_lat > morph_area) {
        morphing.x += morphLat(pos);
    }
    if (dist_lon > morph_area) {
        morphing.y += morphLon(pos);
    }

    return morphing;
}

void main() {
    vec2 local_position = (local_matrix * vec4(position.x, 0, position.y, 1.0)).xz;
    if (lod > 0) {
        local_position += morph(local_position, lod_morph_area[lod-1]);
    }

    tc_uv = local_position;
    float height = texture(heightmap, tc_uv).r;

    vec4 world_pos = world_matrix * vec4(local_position.x, height, local_position.y, 1.0);

    gl_Position = world_pos;
}