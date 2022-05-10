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
		CAMERA_DISTANCE_FROM_ORIGIN * glm::sin(glm::radians(45.0f)) * glm::cos(glm::radians(45.0f)), // x
		CAMERA_DISTANCE_FROM_ORIGIN * glm::cos(glm::radians(45.0f)), // y
		CAMERA_DISTANCE_FROM_ORIGIN * glm::sin(glm::radians(45.0f)) * glm::sin(glm::radians(45.0f)) // z
	);
	const float DEFAULT_CAMERA_SPEED = 3.0f;
	const float DEFAULT_CAMERA_SENSITIVITY = 0.1f;
	const float CAMERA_NEAR_PLANE = 0.1f;
	const float CAMERA_FAR_PLANE = 2000.0f;
	
	const int DEFAULT_OCEAN_WIDTH = 500;
	const int DEFAULT_OCEAN_LENGTH = 500;
	const int DEFAULT_OCEAN_GRID_WIDTH = 500;
	const int DEFAULT_OCEAN_GRID_LENGTH = 500;

	const int SEABED_EXTENSION_FROM_OCEAN = 5;
	const float SEABED_DEPTH_BELOW_OCEAN = 40.0f;
	const int DEFAULT_SEABED_GRID_WIDTH = DEFAULT_OCEAN_GRID_WIDTH / 10 + SEABED_EXTENSION_FROM_OCEAN;
	const int DEFAULT_SEABED_GRID_LENGTH = DEFAULT_OCEAN_GRID_LENGTH / 10 + SEABED_EXTENSION_FROM_OCEAN;

	const float AIR_REFRACTIVE_INDEX = 1.0003f;
	const float WATER_REFRACTIVE_INDEX = 1.3333f;

	const glm::vec3 DEFAULT_WATER_BASE_COLOUR = { 0.02f, 0.13f, 0.25f };
	const float DEFAULT_WATER_BASE_COLOUR_AMOUNT = 0.65f;

	const int DEFAULT_ENV_MAP = 0;  // 0: sky_skybox_1, 1: sky_skybox_2, 3: sunset_skybox_1, 4: sunset_skybox_2, 5: sunset_skybox_3
}

#endif