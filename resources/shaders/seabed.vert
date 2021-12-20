#version 430 core

#define PI 3.14159265358979323846

layout (location = 0) in vec3 oc_pos;
layout (location = 1) in vec2 tex_coords;

out VS_OUT
{
	vec3 wc_pos;
	vec3 wc_normal;
	vec2 tex_coords;
} vs_out;

uniform mat4 m_matrix;
uniform mat4 vp_matrix;
uniform sampler2D perlin_tex;


void main()
{
	vec4 wc_pos = m_matrix * vec4(oc_pos, 1.0);
	vs_out.wc_pos = vec3(wc_pos);

	float displ = texture(perlin_tex, tex_coords).r;
	float delta = 0.001;
	vec4 wc_pos_displaced = vec4(wc_pos.x, wc_pos.y + 20*displ, wc_pos.zw);
	vec3 wc_pos_displaced_dx = vec3(1, 20*(texture(perlin_tex, tex_coords + vec2(delta, 0)).r - displ), 0);
	vec3 wc_pos_displaced_dz = vec3(0, 20*(texture(perlin_tex, tex_coords + vec2(0, delta)).r - displ), 1);

	gl_Position = vp_matrix * wc_pos_displaced;
	vs_out.wc_normal = -cross(wc_pos_displaced_dx, wc_pos_displaced_dz);

	vs_out.tex_coords = tex_coords;
}