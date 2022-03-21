#version 430

layout(quads, equal_spacing, ccw) in;

in VS_OUT
{
	vec3 wc_pos;
	vec2 tex_coords;
} tes_in[];


out VS_OUT
{
    vec3 wc_pos;
	vec3 wc_normal;
	vec2 tex_coords;
} tes_out;

// transformation matrices & time
uniform mat4 vp_matrix;
uniform float time;

// wave simulation parameters
const int NUM_WAVES = 16;
uniform vec2 sim_wavevecs[NUM_WAVES];
uniform float sim_freqs[NUM_WAVES];
uniform float sim_amplitudes[NUM_WAVES];
uniform float sim_phases[NUM_WAVES];

// heightmap functions
float heightmap(vec2 p);		// gives the value of the heightmap at point (x,z)
float heightmap_dx(vec2 p);		// gives the value of the derivative in the x direction, at pt (x,z)
float heightmap_dz(vec2 p);		// like ^^ but for z direction

// horizontal displacementent functions
vec2 horiz_displ(vec2 p);		// gives the horizontal displacement
vec2 horiz_displ_dx(vec2 p);	// gives value of the derivative in the x direction, at pt (x,z)
vec2 horiz_displ_dz(vec2 p);	// gives value of the derivative in the x direction, at pt (x,z)


void main()
{
	// --- interpolate tex coords
    vec2 t0 = mix(tes_in[0].tex_coords, tes_in[1].tex_coords, gl_TessCoord.x);
	vec2 t1 = mix(tes_in[3].tex_coords, tes_in[2].tex_coords, gl_TessCoord.x);
	tes_out.tex_coords = mix(t0, t1, gl_TessCoord.y);

	// --- world coords position & wave simularion
	vec3 p0 = mix(tes_in[0].wc_pos, tes_in[1].wc_pos, gl_TessCoord.x);
	vec3 p1 = mix(tes_in[3].wc_pos, tes_in[2].wc_pos, gl_TessCoord.x);
    vec3 wc_pos = mix(p0, p1, gl_TessCoord.y);

	// calc displaced position
	vec2 horiz_change = horiz_displ(wc_pos.xz);
	vec3 wc_pos_displaced = vec3(	
		wc_pos.x - horiz_change.x, 
		wc_pos.y + heightmap(wc_pos.xz), 
		wc_pos.z - horiz_change.y
	);

	// calc derivatives
	vec2 horiz_change_dx = horiz_displ_dx(wc_pos.xz);
	vec2 horiz_change_dz = horiz_displ_dz(wc_pos.xz);
	vec3 wc_pos_displaced_dx = vec3(1-horiz_change_dx.x, heightmap_dx(wc_pos.xz), horiz_change_dx.y);
	vec3 wc_pos_displaced_dz = vec3(horiz_change_dz.x, heightmap_dz(wc_pos.xz), 1-horiz_change_dz.y);
	
	// --- normal
	tes_out.wc_normal = cross(wc_pos_displaced_dx, wc_pos_displaced_dz);

    // --- transform to screen space
	tes_out.wc_pos = wc_pos_displaced;
    gl_Position = vp_matrix * vec4(wc_pos_displaced, 1.0);
}

float heightmap(vec2 p)
{
	float res = 0;
	for (int i = 0; i < NUM_WAVES; i++)
	{
		res += sim_amplitudes[i] * cos(dot(sim_wavevecs[i], p) - sim_freqs[i] * time + sim_phases[i]);
	}

	return res; 
}

float heightmap_dx(vec2 p)
{
	float res = 0;
	for (int i = 0; i < NUM_WAVES; i++)
	{
		res += sim_amplitudes[i] * sim_wavevecs[i].x * -sin(dot(sim_wavevecs[i], p) - sim_freqs[i] * time + sim_phases[i]);
	}
	return res;
}

float heightmap_dz(vec2 p)
{
	float res = 0;
	for (int i = 0; i < NUM_WAVES; i++)
	{
		res += sim_amplitudes[i] * sim_wavevecs[i].y * -sin(dot(sim_wavevecs[i], p) - sim_freqs[i] * time + sim_phases[i]);
	}
	return res;
}

vec2 horiz_displ(vec2 p)
{
	vec2 res = vec2(0);
	for (int i = 0; i < NUM_WAVES; i++)
	{
		res += normalize(sim_wavevecs[i]) * sim_amplitudes[i] * sin(dot(sim_wavevecs[i], p) - sim_freqs[i] * time + sim_phases[i]);
	}
	return res;
}

vec2 horiz_displ_dx(vec2 p)
{
	vec2 res = vec2(0);
	for (int i = 0; i < NUM_WAVES; i++)
	{
		res += normalize(sim_wavevecs[i]) * sim_amplitudes[i] * sim_wavevecs[i].x * sin(dot(sim_wavevecs[i], p) - sim_freqs[i] * time + sim_phases[i]);
	}
	return res;
}

vec2 horiz_displ_dz(vec2 p)
{
	vec2 res = vec2(0);
	for (int i = 0; i < NUM_WAVES; i++)
	{
		res += normalize(sim_wavevecs[i]) * sim_amplitudes[i] * sim_wavevecs[i].y * sin(dot(sim_wavevecs[i], p) - sim_freqs[i] * time + sim_phases[i]);
	}
	return res;
}
