#version 430 core

#define PI 3.14159265358979323846

layout (location = 0) in vec3 oc_pos;
layout (location = 1) in vec3 oc_normals;

uniform mat4 m_matrix;
uniform mat4 vp_matrix;
uniform float time;

float heightmap(float x, float z);

void main()
{
	vec4 wc_pos = m_matrix * vec4(oc_pos, 1.0);
	gl_Position = vp_matrix * vec4(wc_pos.x, wc_pos.y + heightmap(wc_pos.x, wc_pos.z), wc_pos.zw);
}

float heightmap(float x, float z)
{
	return sin((x + z - time)*0.5);
}