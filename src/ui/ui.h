#ifndef NEREUS_UI
#define NEREUS_UI
#pragma once

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>


namespace UI
{
	void init();
	void render();
	void destroy();
}

#endif
