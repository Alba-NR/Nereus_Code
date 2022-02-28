#version 430 core

// outut patch size
layout (vertices = 4) out;

// attributes for each vertex in input patch
in VS_OUT
{
	vec3 wc_pos;
	vec2 tex_coords;
} tcs_in[];

// attributes for each vertex in output patch
out VS_OUT
{
	vec3 wc_pos;
	vec2 tex_coords;
} tcs_out[];

uniform vec3 wc_camera_pos;

const float MIN_DIST_TO_CAM = 10; // todo: change to uniforms and set based on constants.h
const float MAX_DIST_TO_CAM = 300;
const float DIFF_DIST = 290; // calc manually: max dist - min dist
const float MIN_TESSEL_FACTOR_LOG = 0;
const float MAX_TESSEL_FACTOR_LOG = 6;
const float DIFF_TESEL_LOG = 6;

float calcTesselFactor(float dist_to_cam);

void main()
{
    // set vertices of the output patch (& their attributes)
    tcs_out[gl_InvocationID].wc_pos = tcs_in[gl_InvocationID].wc_pos;
	tcs_out[gl_InvocationID].tex_coords = tcs_in[gl_InvocationID].tex_coords;

    // calc tessellation levels
    
    float dist_to_cam_0 = distance(tcs_in[0].wc_pos, wc_camera_pos);
    float dist_to_cam_1 = distance(tcs_in[1].wc_pos, wc_camera_pos);
    float dist_to_cam_2 = distance(tcs_in[2].wc_pos, wc_camera_pos);
    float dist_to_cam_3 = distance(tcs_in[3].wc_pos, wc_camera_pos);
    
    gl_TessLevelOuter[0] = calcTesselFactor((dist_to_cam_0 + dist_to_cam_1) / 2.0);
    gl_TessLevelOuter[1] = calcTesselFactor((dist_to_cam_0 + dist_to_cam_2) / 2.0);
    gl_TessLevelOuter[2] = calcTesselFactor((dist_to_cam_2 + dist_to_cam_3) / 2.0);
    gl_TessLevelOuter[3] = calcTesselFactor((dist_to_cam_1 + dist_to_cam_3) / 2.0);
    gl_TessLevelInner[0] = (gl_TessLevelOuter[0] + gl_TessLevelOuter[1]) / 2.0;
    gl_TessLevelInner[1] = (gl_TessLevelOuter[2] + gl_TessLevelOuter[3]) / 2.0;
}

float calcTesselFactor(float dist_to_cam)
{
    float tessel_factor = 1.0;
    if (dist_to_cam <= MIN_DIST_TO_CAM)
    {
        tessel_factor = pow(2, MAX_TESSEL_FACTOR_LOG);
    }
    else if (dist_to_cam >= MAX_DIST_TO_CAM)
    {
        tessel_factor = pow(2, MIN_TESSEL_FACTOR_LOG);
    }
    else
    {
        tessel_factor = pow(2, DIFF_TESEL_LOG * (1-(dist_to_cam - MIN_DIST_TO_CAM)/DIFF_DIST) + MIN_TESSEL_FACTOR_LOG);
    }

    return tessel_factor;
}