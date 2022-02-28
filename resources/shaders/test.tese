#version 430

layout(triangles, fractional_even_spacing, ccw) in;

in VS_OUT
{
	vec3 wc_pos;
	vec3 wc_normal;
	vec2 tex_coords;
} tes_in[];


out VS_OUT
{
    vec3 wc_pos;
	vec3 wc_normal;
	vec2 tex_coords;
} tes_out;

uniform mat4 vp_matrix;

vec2 interpolate(vec2 v1, vec2 v2, vec2 v3);
vec3 interpolate(vec3 v1, vec3 v2, vec3 v3);

void main()
{

	// calc output vertex attributes by interpolating (using barycentric coords in gl_TessCoord)
    tes_out.tex_coords = interpolate(tes_in[0].tex_coords, tes_in[1].tex_coords, tes_in[1].tex_coords);
    tes_out.wc_normal = normalize(interpolate(tes_in[0].wc_normal, tes_in[1].wc_normal, tes_in[2].wc_normal));
    tes_out.wc_pos = interpolate(tes_in[0].wc_pos, tes_in[1].wc_pos, tes_in[2].wc_pos);

    // transform to screen space
    gl_Position = vp_matrix * vec4(tes_out.wc_pos, 1.0);
}

vec2 interpolate(vec2 v1, vec2 v2, vec2 v3)
{
    return gl_TessCoord.x * v1 + gl_TessCoord.y * v2 + gl_TessCoord.z * v3;
}

vec3 interpolate(vec3 v1, vec3 v2, vec3 v3)
{
    return gl_TessCoord.x * v1 + gl_TessCoord.y * v2 + gl_TessCoord.z * v3;
}

