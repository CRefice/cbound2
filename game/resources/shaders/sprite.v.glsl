#version 330 core

layout (location = 0) in vec2 vertex;
layout (location = 1) in vec2 tex_coords;

out vec2 tex_coord;

uniform mat4 view_projection;

void main() {
	tex_coord = vec2(tex_coords.x, 1 - tex_coords.y);
	gl_Position = view_projection * vec4(vertex.xy, 0.0, 1.0);
}
