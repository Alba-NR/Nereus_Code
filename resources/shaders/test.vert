#version 430 core

layout (location = 0) in vec3 oc_pos;       // vertex position in object coord

void main()
{
    gl_Position = vec4(oc_pos, 1.0);
}
