#version 430 core

in VS_OUT
{
	vec3 wc_pos;
	vec3 wc_normal;
	vec2 tex_coords;
} fs_in;

layout (pixel_center_integer) in vec4 gl_FragCoord;

out vec4 frag_colour;

uniform vec3 wc_camera_pos;
uniform sampler2D tex_S;

uniform vec2 viewport_dimensions;

const float delta = 0.1;

void main()
{
	vec3 I_result;

	// calculate normal vectors
	vec3 N = normalize(fs_in.wc_normal);

	// calculate projected texture coordinates (screen/vieewport space)
	vec2 projected_tex_coords = gl_FragCoord.xy / viewport_dimensions;

	// calc new sample coords
	vec2 sample_tex_coords = projected_tex_coords + N.xy * delta;

	// set output/final colour
	frag_colour = vec4(sample_tex_coords, 1,1); //texture(tex_S, projected_tex_coords);
}