#version 430

layout(quads, fractional_even_spacing, ccw) in;

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

void main()
{

	// calc output vertex attributes by interpolating (using barycentric coords in gl_TessCoord)

	// tex coords
    vec2 t0 = mix(tes_in[0].tex_coords, tes_in[1].tex_coords, gl_TessCoord.x);
	vec2 t1 = mix(tes_in[2].tex_coords, tes_in[3].tex_coords, gl_TessCoord.x);
	tes_out.tex_coords = mix(t0, t1, gl_TessCoord.y);

	vec3 n0 = mix(tes_in[0].wc_normal, tes_in[1].wc_normal, gl_TessCoord.x);
	vec3 n1 = mix(tes_in[2].wc_normal, tes_in[3].wc_normal, gl_TessCoord.x);
    tes_out.wc_normal = mix(n0, n1, gl_TessCoord.y);

	vec3 p0 = mix(tes_in[0].wc_pos, tes_in[1].wc_pos, gl_TessCoord.x);
	vec3 p1 = mix(tes_in[2].wc_pos, tes_in[3].wc_pos, gl_TessCoord.x);
    tes_out.wc_pos = mix(p0, p1, gl_TessCoord.y);

    // transform to screen space
    gl_Position = vp_matrix * vec4(tes_out.wc_pos, 1.0);
}

