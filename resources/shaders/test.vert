#version 430 core

layout (location = 0) in vec3 oc_pos;

out vec4 colour;

uniform mat4 model_m;

void main()
{
	gl_Position = model_m * vec4(oc_pos, 1.0);
	colour = vec4(1.0, 0.0, 0.0, 1.0);
}