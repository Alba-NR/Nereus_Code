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
	vec3(1.0, 1.0, 1.0),	// colour
	vec3(30, -45, 30),		// direction
	3.0						// strength
);

// Some material constants

// skybox blue: vec4(0.45, 0.63, 0.86, 1.0);
// dark blue: vec4(0.18, 0.28, 0.38, 1.0);
// mid blue: vec4(0.21, 0.47, 0.76, 1.0);
// red: vec4(1.0, 0.25, 0.25, 1.0);

//const vec3 diffuse_colour = vec3(0.45, 0.63, 0.86);		// diffuse intensity/colour
const float K_diff = 0.6;								// diffuse reflection coefficient
const vec3 specular_colour = vec3(0.21, 0.47, 0.76);	// specular highlights intensity/colour
const float K_spec = 0.3;								// specular reflection coeff
const float shininess = 16;								// specular shininess coeff
const vec3 I_a = vec3(0.45, 0.63, 0.86);				// ambient light intensity/colour
const float K_a = 0.75;									// ambient light reflection coeff

uniform vec3 water_base_colour;


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
    vec3 I_diffuse = light.colour * water_base_colour * K_diff * max(dot(N, L), 0.0);
    vec3 I_specular = light.colour * specular_colour * K_spec * pow(max(dot(V, R), 0.0), shininess);

	I_result = (I_diffuse + I_specular) * light.strength;

	// ambient light
    I_result += I_a * water_base_colour * K_a;

	// set output/final colour
	frag_colour = vec4(tonemap(I_result), 1.0);
}
