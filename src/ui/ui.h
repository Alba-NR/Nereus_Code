#ifndef NEREUS_UI
#define NEREUS_UI
#pragma once

#include "../main/app_context.h"

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>


class UI
{
private:
	static Nereus::AppContext *m_app_context;

public:
	static void init(Nereus::AppContext *app_context);
	static void render();
	static void destroy();
};

#endif