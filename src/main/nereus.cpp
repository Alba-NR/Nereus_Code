// Local Headers
#include "nereus.h"
#include "../graphics/core/window_manager.h"
#include "../graphics/shaders.h"
#include "../graphics/meshes/test_mesh.h"

// System Headers
#include <imgui.h>
#include <vector>

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
    return WindowManager::createWindow();
}

void Nereus::renderLoop()
{
    // Test Code
    std::vector<Shader> shaders;
    shaders.emplace_back("test.vert");
    shaders.emplace_back("test.frag");
    ShaderProgram prog(shaders);

    TestMesh mesh;
    mesh.initialise();
    mesh.getVAO().bind();
    prog.bindData(0, mesh.getPositionsVBO(), 3);

    // Rendering Loop
    while (!WindowManager::windowShouldClose())
    {
        if (WindowManager::getKeyState(GLFW_KEY_ESCAPE) == GLFW_PRESS)
            WindowManager::setWindowShouldClose();

        prog.use();       
        mesh.render();

        // flip front & back buffers; and draw
        WindowManager::updateWindow();

        glfwPollEvents();
    }
}

void Nereus::terminate()
{
    glfwTerminate();
}