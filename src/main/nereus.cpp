// Local Headers
#include "nereus.h"
#include "../graphics/core/window_manager.h"

// System Headers
#include <imgui.h>


int main() 
{
    int wasInitOk = Nereus::init(); // load glfw & create window
    if (wasInitOk != EXIT_SUCCESS)  // check if error when init
    {
        return wasInitOk;
    }

    Nereus::renderLoop();   // render geom to window
    Nereus::terminate(); 
    
    return EXIT_SUCCESS;
}

int Nereus::init() 
{
    glfwInit(); // TODO: check if correctly init glfw?
    return WindowManager::createWindow();
}

void Nereus::renderLoop()
{
    // Rendering Loop
    while (!WindowManager::windowShouldClose())
    {
        if (WindowManager::getKeyState(GLFW_KEY_ESCAPE) == GLFW_PRESS)
            WindowManager::setWindowShouldClose();

        WindowManager::clearWindow();

        // flip front & back buffers; and draw
        WindowManager::updateWindow();

        glfwPollEvents();
    }   
}

void Nereus::terminate()
{
    glfwTerminate();
}