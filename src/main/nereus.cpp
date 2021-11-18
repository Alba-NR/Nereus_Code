// Local Headers
#include "nereus.h"
#include "constants.h"
#include "../graphics/shaders.h"
#include "../graphics/meshes/ocean_mesh.h"
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
        UI::init(&m_context);
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

        int last_ocean_mesh_grid_width = m_context.m_ocean_grid_width;
        int last_ocean_mesh_grid_length = m_context.m_ocean_grid_length;
        OceanMesh ocean_mesh(last_ocean_mesh_grid_width, last_ocean_mesh_grid_length);
        ocean_mesh.initialise();
        ocean_mesh.getVAO().bind();
        prog.bindData(0, ocean_mesh.getPositionsVBO(), 3);
        

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

            // --- render ocean mesh ---

            // update mesh data if the grid resolution has been changed in the UI
            if (last_ocean_mesh_grid_width != m_context.m_ocean_grid_width
                || last_ocean_mesh_grid_length != m_context.m_ocean_grid_length)
            {
                ocean_mesh.updateMeshGrid(m_context.m_ocean_grid_width, m_context.m_ocean_grid_length);
                last_ocean_mesh_grid_width = m_context.m_ocean_grid_width;
                last_ocean_mesh_grid_length = m_context.m_ocean_grid_length;
            }

            // transformation matrices
            glm::mat4 model_matrix = glm::mat4(1.0f);
            model_matrix = glm::translate(model_matrix, glm::vec3(-m_context.m_ocean_width / 2, 0.0f, -m_context.m_ocean_length / 2));
            model_matrix = glm::scale(model_matrix, glm::vec3(
                m_context.m_ocean_width / (float)m_context.m_ocean_grid_width,
                1.0f,
                m_context.m_ocean_length / (float)m_context.m_ocean_grid_length
            ));
            
            glm::mat4 mvp_matrix = m_context.m_render_camera.getProjMatrix() * m_context.m_render_camera.getViewMatrix() * model_matrix;

            // render mesh
            prog.use();
            prog.setMat4("mvp_matrix", mvp_matrix);
            ocean_mesh.render();

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
