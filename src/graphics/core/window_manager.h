#ifndef WINDOW_MANAGER
#define WINDOW_MANAGER
#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cstdio>
#include <cstdlib>

#include "../../main/constants.h"

namespace WindowManager
{
	namespace
	{
		// Accessible within the WindowManager namespace only
		const unsigned int SCREEN_WIDTH = NereusConstants::DEFAULT_WINDOW_WIDTH;
		const unsigned int SCREEN_HEIGHT = NereusConstants::DEFAULT_WINDOW_HEIGHT;
		const glm::vec3 bgColour(0.2f, 0.2f, 0.25f);
		GLFWwindow *window = nullptr;
	}

	int createWindow();
	void updateWindow();
	void clearWindow();
	void closeWindow();
	bool windowShouldClose();
	void setWindowShouldClose();

	void setCallbacks();
	unsigned int getKeyState(unsigned int key);

	unsigned int getScreenWidth();
	unsigned int getScreenHeight();

};
#endif 