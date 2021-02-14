#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec4 clipPlane = vec4(0, -1, 0, 5);


out vec4 fragPos;
out vec2 fragUV;

uniform sampler2D heightmap;

void main() {
    vec3 p = position;
    p.y = texture(heightmap, uv).r  * 20.0;
    vec4 worldPos = model * vec4(p, 1.0);
    fragPos = projection * view * worldPos;
    fragUV = uv;
    gl_ClipDistance[0] = dot(worldPos, clipPlane);
    gl_Position = fragPos;
}
