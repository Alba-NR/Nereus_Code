// Local Headers
#include "nereus.h"
#include "window_manager.h"

// System Headers
#include <imgui.h>


int main() 
{
    int wasInitOk = init();         // load glfw & create window
    if (wasInitOk != EXIT_SUCCESS)  // check if error when init
    {
        return wasInitOk;
    }

    renderLoop();   // render geom to window

    glfwTerminate();
    
    return EXIT_SUCCESS;
}

int init() 
{
    glfwInit(); // TODO: check if correctly init glfw?
    return WindowManager::createWindow();
}

void renderLoop()
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