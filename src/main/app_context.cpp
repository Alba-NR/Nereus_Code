
#include "app_context.h"
#include "constants.h"


Nereus::AppContext::AppContext() 
	: m_last_mouse_x(NereusConstants::DEFAULT_WINDOW_WIDTH / 2.0f),
	m_last_mouse_y(NereusConstants::DEFAULT_WINDOW_HEIGHT / 2.0f),
	m_render_camera(NereusConstants::CAMERA_POSITION)
{

}