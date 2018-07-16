#version 400 core

in vec2 texcoord;
out vec4 out_color;

uniform sampler2DArray image;
uniform uint current_frame;

void main() { 
		vec4 color = texture(image, vec3(texcoord.xy, current_frame));
		if (color.a < 0.5) discard;
		out_color = color;
}  
