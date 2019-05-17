#version 330 core

layout (location = 0) in vec4 vertex;
layout (location = 1) in vec2 tex_coords;
layout (location = 2) in vec4 color;

out vec2 tex_coord;
out vec4 mul_color;

layout (std140) uniform Camera
{
	mat4 view;
	mat4 projection;
};

void main() {
	tex_coord = vec2(tex_coords.x, tex_coords.y);
	mul_color = color;
	gl_Position = projection * view * vertex;
}
