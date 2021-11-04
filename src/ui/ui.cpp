
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
		// --- start context for new frame ---
		ImGui_ImplGlfw_NewFrame();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		// --- set up window  & location to top-left corner---
		ImGui::Begin("Nereus");

		const ImGuiViewport *main_viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x, main_viewport->WorkPos.y));

		// --- set up widgets ---
		
		// display fps
		ImGui::Text("Average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		// wireframe mode toggle (on/off)
		static bool wireframe_on = false;
		ImGui::Checkbox("Wireframe mode", &wireframe_on);
		if (wireframe_on)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		
		
		// end window
		ImGui::End();

		// --- render gui to screen ---
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