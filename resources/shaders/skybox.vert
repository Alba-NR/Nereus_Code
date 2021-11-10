#version 430 core

layout (location = 0) in vec3 oc_pos;

out vec3 tex_coords;

uniform mat4 vp_matrix;

void main()
{
    tex_coords = oc_pos;
    vec4 pos = vp_matrix * vec4(oc_pos, 1.0);
    gl_Position = pos.xyww;
}