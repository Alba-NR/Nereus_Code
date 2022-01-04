#version 430 core

in vec3 tex_coords;

out vec4 frag_colour;

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
    frag_colour = vec4(tonemap(texture(env_map, tex_coords).rgb), 1.0);
}

