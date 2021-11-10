// Local Headers
#include "nereus.h"
#include "constants.h"
#include "../graphics/shaders.h"
#include "../graphics/meshes/test_mesh.h"
#include "../graphics/renderers/renderers.h"
#include "../graphics/textures.h"


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
        // ------------------------------
        // Test Mesh
        
        std::vector<Shader> shaders;
        shaders.emplace_back("test.vert");
        shaders.emplace_back("test.frag");
        ShaderProgram prog(shaders);

        TestMesh mesh;
        mesh.initialise();
        mesh.getVAO().bind();
        prog.bindData(0, mesh.getPositionsVBO(), 3);
        

        // ------------------------------
        // Skybox

        // Create skybox shaders
        std::vector<Shader> skybox_shaders;
        skybox_shaders.emplace_back("skybox.vert");
        skybox_shaders.emplace_back("skybox.frag");
        ShaderProgram skybox_shader_prog(skybox_shaders);

        // Create skybox cubemap texture
        string skybox_images[] = {
            "sky_skybox/right.jpg",
            "sky_skybox/left.jpg",
            "sky_skybox/top.jpg",
            "sky_skybox/bottom.jpg",
            "sky_skybox/front.jpg",
            "sky_skybox/back.jpg"            
        };
        SkyBoxRenderer skybox_renderer(skybox_shader_prog, skybox_images);
        skybox_renderer.prepare();


        // ------------------------------
        // Rendering Loop
        while (!m_window.shouldClose())
        {
            // clear window
            m_window.clear();

            // --- render test mesh ---

            // transformation matrices
            glm::mat4 model_matrix = glm::mat4(1.0f);
            //model_matrix = glm::translate(model_matrix, glm::vec3(-2.5f, 0.0f, -2.5f));
            model_matrix = glm::scale(model_matrix, glm::vec3(10.0f, 1.0f, 10.0f));
            model_matrix = glm::rotate(model_matrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            
            glm::mat4 mvp_matrix = m_context.m_render_camera.getProjMatrix() * m_context.m_render_camera.getViewMatrix() * model_matrix;

            // render mesh
            prog.use();
            prog.setMat4("mvp_matrix", mvp_matrix);
            mesh.render();

            // --- render skybox ---
            skybox_renderer.render(m_context.m_render_camera);
            
            // --- render UI ---
            UI::render();

            // --- draw on screen ---
            // flip front & back buffers; and draw
            m_window.update();

            glfwPollEvents();
        }
    }
}
