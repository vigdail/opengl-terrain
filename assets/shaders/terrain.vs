#version 330 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 uv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec4 clipPlane;
uniform float scale_y;

out vec4 fragPos;
out vec2 fragUV;

uniform sampler2D heightmap;

void main() {
    float y = texture(heightmap, uv).r * scale_y;
    vec3 p = vec3(position.x, y, position.y);
    vec4 worldPos = model * vec4(p, 1.0);
    fragPos = projection * view * worldPos;
    fragUV = uv;
    gl_ClipDistance[0] = dot(worldPos, clipPlane);
    gl_Position = fragPos;
}
