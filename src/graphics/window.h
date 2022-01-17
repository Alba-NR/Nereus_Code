#ifndef WINDOW
#define WINDOW
#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>

#include <cstdio>
#include <cstdlib>

#include "../main/constants.h"

class Window
{
private:
	static Window *s_instance;

	const unsigned int SCREEN_WIDTH = NereusConstants::DEFAULT_WINDOW_WIDTH;
	const unsigned int SCREEN_HEIGHT = NereusConstants::DEFAULT_WINDOW_HEIGHT;
	glm::vec3 m_bgColour = glm::vec3(0.2f, 0.2f, 0.25f);
	GLFWwindow *m_window = nullptr;

	Window();
	void createWindow();
	
public:
	static Window &getInstance();
	~Window();

	void initImGuiGLFW();

	void update();
	void clear();
	void close();
	bool shouldClose();
	void setShouldClose();

	void setCallbacks();
	void setWindowUserPointer(void *ptr);

	unsigned int getKeyState(unsigned int key);

	unsigned int getScreenWidth();
	unsigned int getScreenHeight();
};
#endif 