#version 430 core

in vec2 tex_coords;

out vec4 frag_colour;

uniform sampler2D screen_tex;

void main()
{
	frag_colour = texture(screen_tex, tex_coords);
}