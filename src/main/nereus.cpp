// Local Headers
#include "nereus.h"

#include "../graphics/shaders.h"
#include "../graphics/meshes/test_mesh.h"

// System Headers
#include <glm/glm.hpp>
#include <imgui.h>
#include <vector>

int main()
{
    Nereus::NereusApp::initGLFW();
    Nereus::NereusApp nereus; // load glfw & create window
    nereus.renderLoop();   // render geom to window

    return EXIT_SUCCESS;
}

namespace Nereus
{
    NereusApp::NereusApp() : window(Window::getInstance()), render_camera()
    {
        glfwInit();
        this->window.setCallbacks();
    }

    NereusApp::~NereusApp()
    {
        glfwTerminate();
    }

    void NereusApp::initGLFW()
    {
        glfwInit();
    }

    void NereusApp::renderLoop()
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
        while (!this->window.shouldClose())
        {
            this->window.clear();

            prog.use();
            mesh.render();

            // flip front & back buffers; and draw
            this->window.update();

            glfwPollEvents();
        }
    }
}
