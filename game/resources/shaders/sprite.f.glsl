#version 330 core

// FRAGMENT SHADER

uniform sampler2D image;

in vec2 tex_coord;
in vec4 mul_color;
out vec4 out_color;

void main() {
	vec4 color = texture(image, tex_coord);
	if (color.a < 0.5) discard;
	out_color = color * mul_color;
}
