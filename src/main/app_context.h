#ifndef APP_CONTEXT
#define APP_CONTEXT
#pragma once

#include "../graphics/camera.h"


namespace Nereus
{
	class AppContext
	{
	public:
		float m_last_mouse_x;
		float m_last_mouse_y;

		Camera m_render_camera;

		AppContext();
	};
}

#endif
