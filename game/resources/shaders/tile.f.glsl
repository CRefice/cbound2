#version 330 core

// FRAGMENT SHADER

uniform sampler2D image;

in vec2 tex_coord;
out vec4 out_color;

void main() {
	vec4 color = texture(image, tex_coord);
	out_color = color;
}
