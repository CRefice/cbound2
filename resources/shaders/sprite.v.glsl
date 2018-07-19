#version 330 core

uniform mat4 view_projection;

layout (location = 0) in vec2 vertex;
layout (location = 1) in vec2 tex_coords;

out vec2 tex_coord;

void main() {
	texcoord = texcoords;
	gl_Position = view_projection * vec4(vertex.xy, 0.0, 1.0);
}
