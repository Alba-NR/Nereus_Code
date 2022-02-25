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
uniform samplerCube env_map;
uniform sampler2D tex_S;
uniform vec2 viewport_dimensions;
uniform float fresnel_F_0;

uniform vec3 water_base_colour;
uniform float water_base_colour_amt;

const float delta = 0.05;

// tonemapping and display encoding combined
vec3 tonemap(vec3 linear_rgb)
{
    // no tonemap
	// display encoding
    return pow(linear_rgb, vec3(1.0/2.2)); 
}

void main()
{
	vec3 I_result;

	// calculate normal and view vectors
	vec3 N = normalize(fs_in.wc_normal);
    vec3 V = normalize(wc_camera_pos - fs_in.wc_pos);

	// --- reflection ---

	// calc reflected vec
    vec3 R_V = reflect(-V, N);
	// reflected skybox colour
	vec3 I_refl = texture(env_map, R_V).rgb;

	// --- refraction ---

	// calculate projected texture coordinates (screen/vieewport space)
	vec2 projected_tex_coords = gl_FragCoord.xy / viewport_dimensions;
	// calc new sample coords
	vec2 sample_tex_coords = projected_tex_coords + N.xz * delta;

	// refracted colour
	vec3 I_refr = water_base_colour_amt * water_base_colour + (1-water_base_colour_amt) * texture(tex_S, sample_tex_coords).rgb;

	// --- fresnel effect ---

	// calc fresnel term
	float fresnel_coeff = fresnel_F_0 + (1 - fresnel_F_0) * pow(1 -  max(0, dot(-V,N)), 5);

	// set output/final colour
	frag_colour = vec4(tonemap(fresnel_coeff * I_refl + (1 - fresnel_coeff) * I_refr), 1);
}
