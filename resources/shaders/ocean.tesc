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

uniform float TESSEL_RANGE_DIST_TO_CAM_MIN;
uniform float TESSEL_RANGE_DIST_TO_CAM_MAX;
uniform float LOG_MIN_TESSEL_FACTOR;
uniform float LOG_MAX_TESSEL_FACTOR;

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
    
    gl_TessLevelOuter[0] = calcTesselFactor((dist_to_cam_0 + dist_to_cam_1) / 2.0);  // edge 0-1
    gl_TessLevelOuter[1] = calcTesselFactor((dist_to_cam_0 + dist_to_cam_3) / 2.0);  // edge 0-3
    gl_TessLevelOuter[2] = calcTesselFactor((dist_to_cam_2 + dist_to_cam_3) / 2.0);  // edge 2-3
    gl_TessLevelOuter[3] = calcTesselFactor((dist_to_cam_1 + dist_to_cam_2) / 2.0);  // edge 1-2
    gl_TessLevelInner[0] = (gl_TessLevelOuter[1] + gl_TessLevelOuter[3]) / 2.0;  // horiz
    gl_TessLevelInner[1] = (gl_TessLevelOuter[0] + gl_TessLevelOuter[2]) / 2.0;  // vert
}

float calcTesselFactor(float dist_to_cam)
{
    float tessel_factor = 1.0;
    if (dist_to_cam <= TESSEL_RANGE_DIST_TO_CAM_MIN)
    {
        tessel_factor = pow(2, LOG_MAX_TESSEL_FACTOR);
    }
    else if (dist_to_cam >= TESSEL_RANGE_DIST_TO_CAM_MAX)
    {
        tessel_factor = pow(2, LOG_MIN_TESSEL_FACTOR);
    }
    else
    {
        tessel_factor = pow(2, 
        (LOG_MAX_TESSEL_FACTOR - LOG_MIN_TESSEL_FACTOR) 
        * (1 - (dist_to_cam - TESSEL_RANGE_DIST_TO_CAM_MIN) 
                 / (TESSEL_RANGE_DIST_TO_CAM_MAX - TESSEL_RANGE_DIST_TO_CAM_MIN)) 
        + LOG_MIN_TESSEL_FACTOR
        );
    }

    return tessel_factor;
}