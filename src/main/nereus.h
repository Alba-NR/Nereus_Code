#ifndef NEREUS_MAIN
#define NEREUS_MAIN
#pragma once

#include "../graphics/core/window.h"
#include "../ui/ui.h"
#include "../graphics/camera.h"


namespace Nereus
{
	class NereusApp
	{
	public:
		Window m_window;
		Camera m_render_camera;

		NereusApp();	// init
		~NereusApp();	// terminate

		static void initGLFW();
		void renderLoop();
	};
}
#endif