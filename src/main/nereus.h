#ifndef NEREUS_MAIN
#define NEREUS_MAIN
#pragma once

#include "../graphics/window.h"
#include "app_context.h"
#include "../ui/ui.h"


namespace Nereus
{
	class NereusApp
	{
	private:
		Window m_window;
		AppContext m_context;

	public:

		NereusApp();	// init
		~NereusApp();	// terminate

		static void initGLFW();
		void renderLoop();
	};
}
#endif