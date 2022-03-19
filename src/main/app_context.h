#ifndef APP_CONTEXT
#define APP_CONTEXT
#pragma once

#include "../graphics/camera.h"
#include "constants.h"


namespace Nereus
{
	struct AppContext
	{
		float m_last_mouse_x;
		float m_last_mouse_y;

		int m_ocean_width = NereusConstants::DEFAULT_OCEAN_WIDTH;
		int m_ocean_length = NereusConstants::DEFAULT_OCEAN_LENGTH;
		int m_ocean_grid_width = NereusConstants::DEFAULT_OCEAN_GRID_WIDTH;
		int m_ocean_grid_length = NereusConstants::DEFAULT_OCEAN_GRID_LENGTH;

		bool m_do_render_seabed = true;
		bool m_do_render_ocean = true;
		bool m_do_render_ui = true;

		glm::vec3 m_water_base_colour = NereusConstants::DEFAULT_WATER_BASE_COLOUR;
		float m_water_base_colour_amt = NereusConstants::DEFAULT_WATER_BASE_COLOUR_AMOUNT;

		Camera m_render_camera;

		AppContext();
	};
}

#endif
