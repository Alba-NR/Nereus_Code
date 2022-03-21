#version 430 core

in VS_OUT
{
	vec3 wc_pos;
	vec3 wc_normal;
	vec2 tex_coords;
} fs_in;

out vec4 frag_colour;

uniform vec3 wc_camera_pos;

// Main (directional) light in the scene
struct DirectionalLight 
{ 
    vec3 colour;
    vec3 direction;
    float strength;
};

const DirectionalLight light = DirectionalLight(
	vec3(1.0, 1.0, 1.0),		// colour
	vec3(15.0, -5.0, 15.0),		// direction
	1.0							// strength
);

// Some material constants

const vec3 diffuse_colour = vec3(0.37, 0.30, 0.21);		// diffuse intensity/colour
const float K_diff = 0.6;								// diffuse reflection coefficient
const vec3 specular_colour = vec3(0.84, 0.77, 0.63);	// specular highlights intensity/colour
const float K_spec = 0.4;								// specular reflection coeff
const float shininess = 16;								// specular shininess coeff
const vec3 I_a = vec3(0.84, 0.77, 0.63);				// ambient light intensity/colour
const float K_a = 0.75;									// ambient light reflection coeff

uniform int use_seabed_tex;
uniform sampler2D seabed_tex;

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
    vec3 L = normalize(-light.direction);
    vec3 R = reflect(-L, N);


    // diffuse & specular shading
	vec3 diff_col = diffuse_colour;
	if (use_seabed_tex == 1)
	{
		diff_col = texture(seabed_tex, fs_in.tex_coords).rgb;
	}
    vec3 I_diffuse = light.colour * diff_col * K_diff * max(dot(N, L), 0.0);
    vec3 I_specular = light.colour * specular_colour * K_spec * pow(max(dot(V, R), 0.0), shininess);

	I_result = (I_diffuse + I_specular) * light.strength;

	// ambient light
    I_result += I_a * diffuse_colour * K_a;

	// set output/final colour
	frag_colour = vec4(tonemap(I_result), 1.0);
}
