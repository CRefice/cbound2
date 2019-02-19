#version 330 core

layout (location = 0) in vec4 vertex;
layout (location = 1) in vec2 tex_coords;
layout (location = 2) in vec4 color;

out vec2 tex_coord;
out vec4 mul_color;

uniform mat4 view_projection;

void main() {
	tex_coord = vec2(tex_coords.x, 1 - tex_coords.y);
	mul_color = color;
	gl_Position = view_projection * vertex;
}
