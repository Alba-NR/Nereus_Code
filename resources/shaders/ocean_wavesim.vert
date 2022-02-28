#version 430 core

layout (location = 0) in vec3 oc_pos;
layout (location = 1) in vec2 tex_coords;

out VS_OUT
{
	vec3 wc_pos;
	vec2 tex_coords;
} vs_out;

// transformation matrices
uniform mat4 m_matrix;


void main()
{
	vec4 wc_pos = m_matrix * vec4(oc_pos, 1.0);
	vs_out.wc_pos = vec3(wc_pos);
	vs_out.tex_coords = tex_coords;
}