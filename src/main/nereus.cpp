// Local Headers
#include "nereus.h"
#include "constants.h"
#include "../graphics/shaders.h"
#include "../graphics/meshes/test_mesh.h"


// System Headers
#include <glm/glm.hpp>
#include <vector>
#include <iostream>

int main()
{
    Nereus::NereusApp::initGLFW();
    Nereus::NereusApp nereus; // load glfw & create window
    nereus.renderLoop();   // render geom to window

    return EXIT_SUCCESS;
}

namespace Nereus
{
    NereusApp::NereusApp() : m_window(Window::getInstance()), m_context()
    {
        // set window user pointer to the app context
        m_window.setWindowUserPointer(&m_context);

        // set up callbacks
        m_window.setCallbacks();

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
        while (!m_window.shouldClose())
        {
            // clear window
            m_window.clear();

            glm::mat4 model_matrix = glm::mat4(1.0f);
            model_matrix = glm::scale(model_matrix, glm::vec3(10.0f, 1.0f, 10.0f));
            model_matrix = glm::rotate(model_matrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            
            glm::mat4 mvp_matrix = m_context.m_render_camera.getProjMatrix() * m_context.m_render_camera.getViewMatrix() * model_matrix;

            prog.use();
            prog.setMat4("mvp_matrix", mvp_matrix);


            // render mesh
            mesh.render();

            // render UI
            UI::render();

            // flip front & back buffers; and draw
            m_window.update();

            glfwPollEvents();
        }
    }
}
