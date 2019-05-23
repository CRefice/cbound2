#version 330 core

layout (location = 0) in vec4 vertex;
layout (location = 1) in vec2 tex_coords;

out vec2 tex_coord;

layout (std140) uniform Camera
{
	mat4 view;
	mat4 projection;
};

void main() {
	tex_coord = vec2(tex_coords.x, 1 - tex_coords.y);
	gl_Position = projection * view * vertex;
}
