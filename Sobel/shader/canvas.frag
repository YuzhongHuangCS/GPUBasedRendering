#version 330 core

// Laplacian edge detection
//   -1 -1 -1
//   -1  8 -1
//   -1 -1 -1

out vec4 color;
uniform sampler2D canvas;
uniform vec3 iResolution;

void main() {
	vec2 ut = gl_FragCoord.xy / iResolution.xy;
	vec2 uv = vec2(ut.x, 1 - ut.y);
	float xoffset = 1 / iResolution.x;
	float yoffset = 1 / iResolution.y;

	vec4 a0 = texture2D(canvas, uv + vec2(-xoffset, yoffset));
	vec4 a1 = texture2D(canvas, uv + vec2(0, yoffset));
	vec4 a2 = texture2D(canvas, uv + vec2(xoffset, yoffset));
	vec4 a3 = texture2D(canvas, uv + vec2(-xoffset, 0));
	vec4 a4 = texture2D(canvas, uv);
	vec4 a5 = texture2D(canvas, uv + vec2(xoffset, 0));
	vec4 a6 = texture2D(canvas, uv + vec2(-xoffset, -yoffset));
	vec4 a7 = texture2D(canvas, uv + vec2(0, -yoffset));
	vec4 a8 = texture2D(canvas, uv + vec2(xoffset, -yoffset));
	color = 8.0 * (a4 - 0.125 * (a0 + a1 + a2 + a3 + a5 + a6 + a7 + a8));
}