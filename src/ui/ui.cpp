
#include "ui.h"
#include "../graphics/window.h"
#include "../main/constants.h"

Nereus::AppContext *UI::m_app_context = nullptr;

// Set up Dear ImGui context, backends/platforms & style
void UI::init(Nereus::AppContext *app_context)
{
	// set nereus app context
	UI::m_app_context = app_context;
	 
	// setup Dear ImGui context
	ImGui::CreateContext();

	// set up backend (renderer/platoform) bindings
	Window::getInstance().initImGuiGLFW();
	ImGui_ImplOpenGL3_Init();

	// set up ImGui style
	ImGui::StyleColorsDark();
}

void UI::render()
{
	// --- start context for new frame ---
	ImGui_ImplGlfw_NewFrame();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();

	// --- set up window  & location to top-left corner---
	ImGui::SetNextWindowPos(ImVec2(0, 0));

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoMove;
	ImGui::Begin("Nereus", nullptr, window_flags);

	// --- set up widgets ---
		
	// display fps
	ImGui::Text("Average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	// display number of primitives rendered
	ImGui::Text("Ocean primitives: %i", m_app_context->m_num_ocean_primitives);
	ImGui::Text("Seabed primitives: %i", m_app_context->m_num_seabed_primitives);
	ImGui::Separator();

	// --- render options
	ImGui::Text("Render Options:");

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

	// render seabed mesh toggle (on/off)
	ImGui::Checkbox("Render Seabed", &(m_app_context->m_do_render_seabed));

	// render ocean mesh toggle (on/off)
	ImGui::Checkbox("Render Ocean", &(m_app_context->m_do_render_ocean));

	ImGui::Separator();

	// --- camera options
	ImGui::Text("Camera:");
	// polar angle
	float polar = m_app_context->m_render_camera.getPolarAngle();
	ImGui::InputFloat("Polar", &polar);
	m_app_context->m_render_camera.setPolarAngle(polar);
	// azimuthal angle
	float azimuthal = m_app_context->m_render_camera.getAzimuthalAngle();
	ImGui::InputFloat("Azimuthal", &azimuthal);
	m_app_context->m_render_camera.setAzimuthalAngle(azimuthal);
	// position
	glm::vec3 cam_pos = m_app_context->m_render_camera.getPosition();
	float cam_pos_a[3] = { cam_pos.x, cam_pos.y, cam_pos.z };
	ImGui::InputFloat3("Position", cam_pos_a);
	m_app_context->m_render_camera.setPosition(glm::vec3(
		cam_pos_a[0],
		cam_pos_a[1],
		cam_pos_a[2]
	));
	// fov
	float fov = m_app_context->m_render_camera.getFOV();
	ImGui::InputFloat("FOV", &fov);
	m_app_context->m_render_camera.setFOV(fov);

	ImGui::Separator();

	// --- seabed 
	// seabed mesh
	ImGui::Text("Seabed Mesh:");
	ImGui::PushItemWidth(50);
	ImGui::InputInt("Seabed Grid Width", &(m_app_context->m_seabed_grid_width), 0);
	ImGui::InputInt("Seabed Grid Length", &(m_app_context->m_seabed_grid_length), 0);
	m_app_context->m_seabed_grid_width = max(m_app_context->m_seabed_grid_width, 1);
	m_app_context->m_seabed_grid_length = max(m_app_context->m_seabed_grid_length, 1);
	ImGui::PopItemWidth();

	// seabed texture
	ImGui::Text("Seabed Texture:");
	ImGui::Combo("Tex", &(m_app_context->m_seabed_tex), "none\0sand_seabed_1\0sand_seabed_2\0pretrified_seabed");

	ImGui::Separator();

	// --- ocean 
	// ocean mesh
	ImGui::Text("Ocean Mesh:");
	ImGui::PushItemWidth(50);
	ImGui::InputInt("Ocean Width", &(m_app_context->m_ocean_width), 0);
	ImGui::InputInt("Ocean Length", &(m_app_context->m_ocean_length), 0);
	ImGui::InputInt("Ocean Grid Width", &(m_app_context->m_ocean_grid_width), 0);
	ImGui::InputInt("Ocean Grid Length", &(m_app_context->m_ocean_grid_length), 0);
	m_app_context->m_ocean_width = max(m_app_context->m_ocean_width, 1);
	m_app_context->m_ocean_length = max(m_app_context->m_ocean_length, 1);
	m_app_context->m_ocean_grid_width = max(m_app_context->m_ocean_grid_width, 1);
	m_app_context->m_ocean_grid_length = max(m_app_context->m_ocean_grid_length, 1);
	ImGui::PopItemWidth();

	// illumination model
	ImGui::Text("Illumination Model:");
	ImGui::Combo("Model", &(m_app_context->m_illumin_model), "Fresnel\0Reflection\0Refraction\0Phong\0");

	// base colour
	ImGui::Text("Water Base Colour:");
	static float water_base_colour[3] = {
		m_app_context->m_water_base_colour.r,
		m_app_context->m_water_base_colour.g,
		m_app_context->m_water_base_colour.b
	};
	ImGui::ColorEdit3("", water_base_colour);
	m_app_context->m_water_base_colour = glm::vec3(
		water_base_colour[0],
		water_base_colour[1],
		water_base_colour[2]
	);
	ImGui::SliderFloat("Amount", &(m_app_context->m_water_base_colour_amt), 0.0f, 1.0f, "%.3f");

	// --- skybox
	ImGui::Text("Environment Map:");
	ImGui::Combo("Env.Map", &(m_app_context->m_env_map), "sky_skybox_1\0sky_skybox_2\0sunset_skybox_1\0sunset_skybox_2\0sunset_skybox_3");
		
	// --- end window
	ImGui::End();

	// --- render gui to screen ---
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UI::destroy()
{
	// shutdown backends
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}