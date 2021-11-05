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

	const float CAMERA_DISTANCE_FROM_ORIGIN = 10.0f;
	const glm::vec3 CAMERA_POSITION = glm::vec3(
		CAMERA_DISTANCE_FROM_ORIGIN * glm::cos(glm::radians(60.0f)) * glm::cos(glm::radians(30.0f)), // x
		CAMERA_DISTANCE_FROM_ORIGIN * glm::cos(glm::radians(60.0f)) * glm::sin(glm::radians(30.0f)), // z
		CAMERA_DISTANCE_FROM_ORIGIN * glm::sin(glm::radians(60.0f)) // y
	);
	
}

#endif