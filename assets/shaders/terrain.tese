#version 450 core

layout(quads, fractional_odd_spacing, cw) in;

uniform mat4 view;
uniform mat4 projection;

void main() {
	float u = gl_TessCoord.x;
	float v = gl_TessCoord.y;

	// world position
	vec4 position =
	((1 - u) * (1 - v) * gl_in[12].gl_Position +
	u * (1 - v) * gl_in[0].gl_Position +
	u * v * gl_in[3].gl_Position +
	(1 - u) * v * gl_in[15].gl_Position);

	gl_Position = projection * view * position;
}
