#version 330 core

// FRAGMENT SHADER

in vec2 tex_coord;
in vec4 mul_color;
out vec4 out_color;

void main() {
	out_color = vec4(1, 0, 0, 0.5);
}
