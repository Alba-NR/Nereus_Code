#ifndef APP_CONTEXT
#define APP_CONTEXT
#pragma once

#include "../graphics/camera.h"


namespace Nereus
{
	struct AppContext
	{
		float m_last_mouse_x;
		float m_last_mouse_y;

		int m_ocean_width = 100;
		int m_ocean_height = 50;
		int m_ocean_grid_width = 1;
		int m_ocean_grid_height = 1;

		Camera m_render_camera;

		AppContext();
	};
}

#endif
