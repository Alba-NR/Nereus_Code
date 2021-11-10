#version 430 core

out vec4 frag_colour;

void main()
{
	// skybox blue: vec4(0.45, 0.63, 0.86, 1.0);
	// dark blue: vec4(0.18, 0.28, 0.38, 1.0);
	// mid blue: vec4(0.21, 0.47, 0.76, 1.0);
	// red: vec4(1.0, 0.25, 0.25, 1.0);
	frag_colour = vec4(0.21, 0.47, 0.76, 1.0);
}
