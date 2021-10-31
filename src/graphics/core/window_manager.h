#ifndef WINDOW_MANAGER
#define WINDOW_MANAGER
#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cstdio>
#include <cstdlib>

namespace WindowManager
{
	int createWindow();
	void updateWindow();
	void clearWindow();
	void closeWindow();
	bool windowShouldClose();
	void setWindowShouldClose();
	unsigned int getKeyState(unsigned int key);

	unsigned int getScreenWidth();
	unsigned int getScreenHeight();
};
#endif 