#version 430 core

// outut patch size
layout (vertices = 3) out;

// attributes for each vertex in input patch
in VS_OUT
{
	vec3 wc_pos;
	vec3 wc_normal;
	vec2 tex_coords;
} tcs_in[];

// attributes for each vertex in output patch
out VS_OUT
{
	vec3 wc_pos;
	vec3 wc_normal;
	vec2 tex_coords;
} tcs_out[];


void main()
{
    // set vertices of the output patch (& their attributes)
    tcs_out[gl_InvocationID].wc_pos = tcs_in[gl_InvocationID].wc_pos;
    tcs_out[gl_InvocationID].wc_normal = tcs_in[gl_InvocationID].wc_normal;
	tcs_out[gl_InvocationID].tex_coords = tcs_in[gl_InvocationID].tex_coords;

    // calc tessellation levels
    gl_TessLevelOuter[0] = 2.0;
    gl_TessLevelOuter[1] = 2.0;
    gl_TessLevelOuter[2] = 2.0;
    gl_TessLevelInner[0] = 3.0;
}