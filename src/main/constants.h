#ifndef NEREUS_CONSTANTS
#define NEREUS_CONSTANTS
#pragma once

#include <glm/glm.hpp>


namespace NereusConstants
{
	const float DEFAULT_WINDOW_WIDTH = 1200.0f;
	const float DEFAULT_WINDOW_HEIGHT = 800.0f;

	const unsigned int OPENGL_VERSION_MAJOR = 4;
	const unsigned int OPENGL_VERSION_MINOR = 3;

	const float CAMERA_DISTANCE_FROM_ORIGIN = 50.0f;
	const glm::vec3 CAMERA_POSITION = glm::vec3(
		CAMERA_DISTANCE_FROM_ORIGIN * glm::cos(glm::radians(60.0f)) * glm::cos(glm::radians(30.0f)), // x
		CAMERA_DISTANCE_FROM_ORIGIN * glm::cos(glm::radians(60.0f)) * glm::sin(glm::radians(30.0f)), // z
		CAMERA_DISTANCE_FROM_ORIGIN * glm::sin(glm::radians(60.0f)) // y
	);
	const float DEFAULT_CAMERA_SPEED = 3.0f;
	const float DEFAULT_CAMERA_SENSITIVITY = 0.1f;
	const float CAMERA_NEAR_PLANE = 0.1f;
	const float CAMERA_FAR_PLANE = 250.0f;
	
	const int DEFAULT_OCEAN_WIDTH = 100;
	const int DEFAULT_OCEAN_LENGTH = 50;
	const int DEFAULT_OCEAN_GRID_WIDTH = 50;
	const int DEFAULT_OCEAN_GRID_LENGTH = 50;
}

#endif