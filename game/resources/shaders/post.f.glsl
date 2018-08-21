#version 330 core

out vec4 out_color;
  
in vec2 texcoord;

uniform sampler2D screen_tex;

void main() { 
	vec2 offsets[8] = vec2[](
			vec2(-0.5, -0.5),
			vec2(0.1, -0.2),
			vec2(0.05, 0.4),
			vec2(-0.2, 0.0),
			vec2(-0.45, 0.5),
			vec2(0.0, -0.15),
			vec2(0.2, 0.25),
			vec2(0.5, 0.5)
			);

	vec2 dx = dFdx(texcoord);
	vec2 dy = dFdy(texcoord);
	vec4 color = vec4(0, 0, 0, 0);
	for(int i = 0; i < 8; ++i) {
		color += texture(screen_tex, texcoord + dx * offsets[i].x + dy * offsets[i].y);
	}
	color /= 8;
	out_color = texture(screen_tex, texcoord);//color;
}
