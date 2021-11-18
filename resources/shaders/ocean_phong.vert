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
uniform mat4 normals_matrix;
uniform float time;

float heightmap(float x, float z);		// gives the value of the heightmap at point (x,z)
float heightmap_dx(float x, float z);	// gives the value of the derivative in the x direction, at pt (x,z)
float heightmap_dz(float x, float z);	// like ^^ but for z direction


void main()
{
	vec4 wc_pos = m_matrix * vec4(oc_pos, 1.0);
	vs_out.wc_pos = vec3(wc_pos);

	vec4 wc_pos_displaced = vec4(wc_pos.x, wc_pos.y + heightmap(wc_pos.x, wc_pos.z), wc_pos.zw);
	vec3 wc_pos_displaced_dx = vec3(1, heightmap_dx(wc_pos.x, wc_pos.z), 0);
	vec3 wc_pos_displaced_dz = vec3(0, heightmap_dz(wc_pos.x, wc_pos.z), 1);
	
	gl_Position = vp_matrix * wc_pos_displaced;
	//vs_out.wc_normal = mat3(normals_matrix) * oc_normal;
	vs_out.wc_normal = cross(wc_pos_displaced_dx, wc_pos_displaced_dz);

	vs_out.tex_coords = tex_coords;
}

float heightmap(float x, float z)
{
	return sin((x + z - time)*0.5);
}

float heightmap_dx(float x, float z)
{
	return -cos((x + z - time)*0.5) * 0.5;
}

float heightmap_dz(float x, float z)
{
	return -cos((x + z - time)*0.5) * 0.5;
}