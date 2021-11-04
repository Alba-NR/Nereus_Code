
#include "ui.h"
#include "../graphics/core/window.h"
#include "../main/constants.h"

namespace UI
{
	// Set up Dear ImGui context, backends/platforms & style
	void init()
	{
		// setup Dear ImGui context
		ImGui::CreateContext();

		// set up backend (renderer/platoform) bindings
		Window::getInstance().initImGuiGLFW();
		ImGui_ImplOpenGL3_Init();

		// set up ImGui style
		ImGui::StyleColorsDark();
	}

	void render()
	{
		// start context for new frame
		ImGui_ImplGlfw_NewFrame();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		// set up widgets
		// TODO

		// render gui to screen
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void destroy()
	{
		 // shutdown backends
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
}