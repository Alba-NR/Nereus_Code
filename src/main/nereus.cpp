// Local Headers
#include "nereus.h"
#include "constants.h"
#include "../graphics/shaders.h"
#include "../graphics/renderers/renderers.h"
#include "../graphics/textures.h"


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
            "sky_skybox/right.jpg",
            "sky_skybox/left.jpg",
            "sky_skybox/top.jpg",
            "sky_skybox/bottom.jpg",
            "sky_skybox/front.jpg",
            "sky_skybox/back.jpg"
        };

        // Create Skybox renderer
        SkyBoxRenderer skybox_renderer(skybox_shader_prog, skybox_images);


        // ------------------------------
        // Ocean

        // Create ocean surface shaders
        std::vector<Shader> ocean_shaders;
        ocean_shaders.emplace_back("ocean_wavesim.vert");
        ocean_shaders.emplace_back("ocean_refl.frag");
        ShaderProgram ocean_shader_prog(ocean_shaders);

        // Create ocean renderer
        ReflectiveOceanRenderer ocean_renderer(ocean_shader_prog, skybox_renderer.getCubeMapTexture());

        // Track last ocean size values
        int last_ocean_width = NereusConstants::DEFAULT_OCEAN_WIDTH;
        int last_ocean_length = NereusConstants::DEFAULT_OCEAN_LENGTH;
        int last_ocean_mesh_grid_width = NereusConstants::DEFAULT_OCEAN_GRID_WIDTH;
        int last_ocean_mesh_grid_length = NereusConstants::DEFAULT_OCEAN_GRID_LENGTH;


        // ------------------------------
        // Seabed

        // Create seabed shaders
        std::vector<Shader> seabed_shaders;
        seabed_shaders.emplace_back("seabed.vert");
        seabed_shaders.emplace_back("seabed.frag");
        ShaderProgram seabed_shader_prog(seabed_shaders);

        // Load & create Perlin noise texture
        Texture2D perlin_tex = Texture2D("perlin_noise.jpg");

        // Create ocean renderer
        SeabedRenderer seabed_renderer(seabed_shader_prog, perlin_tex);

        
        // ------------------------------
        // Rendering Loop
        while (!m_window.shouldClose())
        {
            // clear window
            m_window.clear();

            // --- process keyboard input to change camera pos ---
            if (m_window.getKeyState(GLFW_KEY_W) == GLFW_PRESS) // forwards
            {
                m_context.m_render_camera.move(CameraMovement::FORWARDS, ImGui::GetIO().DeltaTime); 
            }
            if (m_window.getKeyState(GLFW_KEY_S) == GLFW_PRESS) // backwards
            {
                m_context.m_render_camera.move(CameraMovement::BACKWARDS, ImGui::GetIO().DeltaTime);
            }
            if (m_window.getKeyState(GLFW_KEY_A) == GLFW_PRESS) // left
            {
                m_context.m_render_camera.move(CameraMovement::RIGHT, ImGui::GetIO().DeltaTime);
            }
            if (m_window.getKeyState(GLFW_KEY_D) == GLFW_PRESS) // right
            {
                m_context.m_render_camera.move(CameraMovement::LEFT, ImGui::GetIO().DeltaTime);
            }
            if (m_window.getKeyState(GLFW_KEY_SPACE) == GLFW_PRESS) // upwards
            {
                m_context.m_render_camera.move(CameraMovement::UPWARDS, ImGui::GetIO().DeltaTime);
            }
            if (m_window.getKeyState(GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) // downwards
            {
                m_context.m_render_camera.move(CameraMovement::DOWNWARDS, ImGui::GetIO().DeltaTime);
            }
       

            // --- update mesh data if changed in UI ---

            // update mesh data if the grid resolution has been changed in the UI
            if (last_ocean_mesh_grid_width != m_context.m_ocean_grid_width
                || last_ocean_mesh_grid_length != m_context.m_ocean_grid_length)
            {
                ocean_renderer.updateOceanMeshGrid(m_context.m_ocean_grid_width, m_context.m_ocean_grid_length);
                seabed_renderer.updateSeabedMeshGrid(m_context.m_ocean_grid_width, m_context.m_ocean_grid_length);
                last_ocean_mesh_grid_width = m_context.m_ocean_grid_width;
                last_ocean_mesh_grid_length = m_context.m_ocean_grid_length;
            }

            // update ocean size info if the size has been changed in the UI
            if (last_ocean_width != m_context.m_ocean_width)
            {
                ocean_renderer.setOceanWidth(m_context.m_ocean_width);
                seabed_renderer.setSeabedWidth(m_context.m_ocean_width + NereusConstants::SEABED_EXTENSION_FROM_OCEAN);
                last_ocean_width = m_context.m_ocean_width;
            }
            if (last_ocean_length != m_context.m_ocean_length)
            {
                ocean_renderer.setOceanLength(m_context.m_ocean_length);
                seabed_renderer.setSeabedLength(m_context.m_ocean_length + NereusConstants::SEABED_EXTENSION_FROM_OCEAN);
                last_ocean_length = m_context.m_ocean_length;
            }

            // --- render seabed ---
            if (m_context.m_do_render_seabed)
            {
                seabed_renderer.render(m_context.m_render_camera);
            }

            // --- render ocean ---
            if (m_context.m_do_render_ocean)
            {
                ocean_renderer.render(m_context.m_render_camera);
            }

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
