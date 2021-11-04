// Local Headers
#include "nereus.h"
#include "../graphics/shaders.h"
#include "../graphics/meshes/test_mesh.h"


// System Headers
#include <glm/glm.hpp>
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
        // set up callbacks
        this->window.setCallbacks();

        // init GUI
        UI::init();
    }

    NereusApp::~NereusApp()
    {
        // note: will call Window destructor & correctly destroy the window

        // cleanup UI resources
        UI::destroy();
        
        // terminate GLFW
        glfwTerminate();       
    }

    // Init GLFW. MUST be called BEFORE creating a NereusApp instance.
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
            // clear window
            this->window.clear();

            // render mesh
            prog.use();
            mesh.render();

            // render UI
            UI::render();

            // flip front & back buffers; and draw
            this->window.update();

            glfwPollEvents();
        }
    }
}
