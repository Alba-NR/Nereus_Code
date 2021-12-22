#version 430 core

layout (location = 0) in vec2 oc_pos;
layout (location = 1) in vec2 in_tex_coords;

out vec2 tex_coords;

void main()
{
	gl_Position = vec4(oc_pos, 0.0, 1.0);
	tex_coords = in_tex_coords;
}