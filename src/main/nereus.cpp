// Local Headers
#include "nereus.h"
#include "constants.h"
#include "../graphics/shaders.h"
#include "../graphics/meshes/ocean_mesh.h"
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
        // Skybox

        // Create skybox shaders
        std::vector<Shader> skybox_shaders;
        skybox_shaders.emplace_back("skybox.vert");
        skybox_shaders.emplace_back("skybox.frag");
        ShaderProgram skybox_shader_prog(skybox_shaders);

        // Create skybox cubemap texture
        string skybox_images[] = {
            "test_skybox/right.jpg",
            "test_skybox/left.jpg",
            "test_skybox/top.jpg",
            "test_skybox/bottom.jpg",
            "test_skybox/front.jpg",
            "test_skybox/back.jpg"
        };

        // Create Skybox renderer
        SkyBoxRenderer skybox_renderer(skybox_shader_prog, skybox_images);


        // ------------------------------
        // Ocean

        // Create ocean surface shaders
        std::vector<Shader> shaders;
        shaders.emplace_back("ocean_wavesim.vert");
        shaders.emplace_back("ocean_refl.frag");
        ShaderProgram ocean_shader_prog(shaders);

        // Create ocean renderer
        OceanRenderer ocean_renderer(ocean_shader_prog, skybox_renderer.getCubeMapTexture());

        // Track last ocean size values
        int last_ocean_width = NereusConstants::DEFAULT_OCEAN_WIDTH;
        int last_ocean_length = NereusConstants::DEFAULT_OCEAN_LENGTH;
        int last_ocean_mesh_grid_width = NereusConstants::DEFAULT_OCEAN_GRID_WIDTH;
        int last_ocean_mesh_grid_length = NereusConstants::DEFAULT_OCEAN_GRID_LENGTH;

        
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
                ocean_renderer.updateOceanMeshGrid(m_context.m_ocean_grid_width, m_context.m_ocean_grid_length);
                last_ocean_mesh_grid_width = m_context.m_ocean_grid_width;
                last_ocean_mesh_grid_length = m_context.m_ocean_grid_length;
            }

            // update ocean size info if the size has been changed in the UI
            if (last_ocean_width != m_context.m_ocean_width)
            {
                ocean_renderer.setOceanWidth(m_context.m_ocean_width);
                last_ocean_width = m_context.m_ocean_width;
            }
            if (last_ocean_length != m_context.m_ocean_length)
            {
                ocean_renderer.setOceanLength(m_context.m_ocean_length);
                last_ocean_length = m_context.m_ocean_length;
            }

            // render ocean
            ocean_renderer.render(m_context.m_render_camera);

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
