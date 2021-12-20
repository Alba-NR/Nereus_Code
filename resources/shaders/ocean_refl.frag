#version 430 core

in VS_OUT
{
	vec3 wc_pos;
	vec3 wc_normal;
	vec2 tex_coords;
} fs_in;

out vec4 frag_colour;

uniform vec3 wc_camera_pos;
uniform samplerCube env_map;


void main()
{
	vec3 I_result;

	// calculate normal and view vectors
	vec3 N = normalize(fs_in.wc_normal);
    vec3 V = normalize(wc_camera_pos - fs_in.wc_pos);
    vec3 R_V = reflect(-V, N);

	// reflected skybox colour
	vec4 I_refl = texture(env_map, R_V);

	// set output/final colour
	frag_colour = I_refl;
}
