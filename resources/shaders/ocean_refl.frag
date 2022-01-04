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


// tonemapping and display encoding combined
vec3 tonemap(vec3 linear_rgb)
{
    // simple tonemapping controlling the brightness only
    float L_white = 0.7;

    float inverse_gamma = 1.0/2.2;  // for display encoding
    return pow(linear_rgb/L_white, vec3(inverse_gamma)); 
}

void main()
{
	vec3 I_result;

	// calculate normal and view vectors
	vec3 N = normalize(fs_in.wc_normal);
    vec3 V = normalize(wc_camera_pos - fs_in.wc_pos);
    vec3 R_V = reflect(-V, N);

	// reflected skybox colour
	vec3 I_refl = texture(env_map, R_V).rgb;

	// set output/final colour
	frag_colour = vec4(tonemap(I_refl), 1.0);
}
