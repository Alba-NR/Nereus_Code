#version 430 core

in vec3 tex_coords;

out vec4 frag_colour;

uniform samplerCube env_map;


void main()
{
    frag_colour = texture(env_map, tex_coords);
}

