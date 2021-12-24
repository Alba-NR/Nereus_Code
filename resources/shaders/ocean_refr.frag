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

uniform vec4 water_base_colour;
uniform float water_base_colour_amt;

const float delta = 0.05;

void main()
{
	vec3 I_result;

	// calculate normal vectors
	vec3 N = normalize(fs_in.wc_normal);

	// calculate projected texture coordinates (screen/vieewport space)
	vec2 projected_tex_coords = gl_FragCoord.xy / viewport_dimensions;

	// calc new sample coords
	vec2 sample_tex_coords = projected_tex_coords + N.xz * delta;

	// set output/final colour
	frag_colour = water_base_colour_amt * water_base_colour + (1-water_base_colour_amt) * texture(tex_S, sample_tex_coords);
}