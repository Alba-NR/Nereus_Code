#version 430 core

layout (location = 0) in vec3 oc_pos;
layout (location = 1) in vec2 tex_coords;

out VS_OUT
{
	vec3 wc_pos;
	vec3 wc_normal;
	vec2 tex_coords;
} vs_out;

// transformation matrices & time
uniform mat4 m_matrix;
uniform mat4 vp_matrix;
uniform float time;

// wave simulation parameters
// for testing out 1 wave:
const float wavelength = 2;
const vec2 wavevector = vec2(1);
const vec2 wavevector_norm = normalize(wavevector);
const float freq = 3;
const float amplitude = 0.5;
const float phase = 1;

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
	vec4 wc_pos = m_matrix * vec4(oc_pos, 1.0);
	vs_out.wc_pos = vec3(wc_pos);

	// calc displaced position
	vec2 horiz_change = horiz_displ(wc_pos.xz);
	vec4 wc_pos_displaced = vec4(	
		wc_pos.x - horiz_change.x, 
		wc_pos.y + heightmap(wc_pos.xz), 
		wc_pos.z - horiz_change.y,
		wc_pos.w
	);

	// calc derivatives
	vec2 horiz_change_dx = horiz_displ_dx(wc_pos.xz);
	vec2 horiz_change_dz = horiz_displ_dz(wc_pos.xz);
	vec3 wc_pos_displaced_dx = vec3(1-horiz_change_dx.x, heightmap_dx(wc_pos.xz), horiz_change_dx.y);
	vec3 wc_pos_displaced_dz = vec3(horiz_change_dz.x, heightmap_dz(wc_pos.xz), 1-horiz_change_dz.y);
	
	// outputs
	gl_Position = vp_matrix * wc_pos_displaced;
	vs_out.wc_normal = cross(wc_pos_displaced_dx, wc_pos_displaced_dz);
	vs_out.tex_coords = tex_coords;
}

float heightmap(vec2 p)
{
	return amplitude * cos(dot(wavevector, p) - freq * time + phase);
}

float heightmap_dx(vec2 p)
{
	return amplitude * wavevector.x * -sin(dot(wavevector, p) - freq * time + phase);
}

float heightmap_dz(vec2 p)
{
	return amplitude * wavevector.y * -sin(dot(wavevector, p) - freq * time + phase);
}

vec2 horiz_displ(vec2 p)
{
	return wavevector_norm * amplitude * sin(dot(wavevector, p) - freq * time + phase);
}

vec2 horiz_displ_dx(vec2 p)
{
	return wavevector_norm * amplitude * wavevector.x * cos(dot(wavevector, p) - freq * time + phase);
}

vec2 horiz_displ_dz(vec2 p)
{
	return wavevector_norm * amplitude * wavevector.y * cos(dot(wavevector, p) - freq * time + phase);
}
