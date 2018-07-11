#version 400 core

layout (location = 0) in vec2 vertex;
layout (location = 1) in vec2 texcoords;

out vec2 texcoord;

uniform mat4 model;
uniform mat4 projection;

void main() {
	texcoord = texcoords;
	gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);
}

