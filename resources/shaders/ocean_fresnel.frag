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

const float delta = 0.05;
const vec4 base_water_colour = vec4(0.21, 0.47, 0.76, 1.0);
const float base_colour_amount = 0.5;


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
	vec4 I_refl = texture(env_map, R_V);

	// --- refraction ---

	// calculate projected texture coordinates (screen/vieewport space)
	vec2 projected_tex_coords = gl_FragCoord.xy / viewport_dimensions;
	// calc new sample coords
	vec2 sample_tex_coords = projected_tex_coords + N.xz * delta;

	// refracted colour
	vec4 I_refr = base_colour_amount * base_water_colour + (1-base_colour_amount) * texture(tex_S, sample_tex_coords);

	// --- fresnel effect ---

	// calc fresnel term
	float fresnel_coeff = fresnel_F_0 + (1 - fresnel_F_0) * pow(1 -  max(0, dot(-V,N)), 5);

	// set output/final colour
	frag_colour = fresnel_coeff * I_refl + (1 - fresnel_coeff) * I_refr;
}
