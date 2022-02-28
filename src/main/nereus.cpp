// Local Headers
#include "nereus.h"
#include "constants.h"
#include "../graphics/shaders.h"
#include "../graphics/renderers.h"
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
            "sunset_skybox_1/right.jpg",
            "sunset_skybox_1/left.jpg",
            "sunset_skybox_1/top.jpg",
            "sunset_skybox_1/bottom.jpg",
            "sunset_skybox_1/front.jpg",
            "sunset_skybox_1/back.jpg"
        };

        // Create Skybox renderer
        SkyBoxRenderer skybox_renderer(skybox_shader_prog, skybox_images);


        // ------------------------------
        // Ocean

        // Create ocean surface shaders
        std::vector<Shader> ocean_shaders;
        ocean_shaders.emplace_back("ocean_wavesim.vert");
        ocean_shaders.emplace_back("ocean_fresnel.frag");
        ocean_shaders.emplace_back("test.tesc");
        ocean_shaders.emplace_back("test.tese");
        ShaderProgram ocean_shader_prog(ocean_shaders);

        // Create ocean renderer
        FullOceanRenderer ocean_renderer(ocean_shader_prog, skybox_renderer.getCubeMapTexture());

        // Track last ocean size values
        int last_ocean_width = NereusConstants::DEFAULT_OCEAN_WIDTH;
        int last_ocean_length = NereusConstants::DEFAULT_OCEAN_LENGTH;
        int last_ocean_mesh_grid_width = NereusConstants::DEFAULT_OCEAN_GRID_WIDTH;
        int last_ocean_mesh_grid_length = NereusConstants::DEFAULT_OCEAN_GRID_LENGTH;

        // Track last water base colour params
        glm::vec3 last_water_base_colour = NereusConstants::DEFAULT_WATER_BASE_COLOUR;
        float last_water_base_colour_amt = NereusConstants::DEFAULT_WATER_BASE_COLOUR_AMOUNT;


        // ------------------------------
        // Seabed

        // Create seabed shaders
        std::vector<Shader> seabed_shaders;
        seabed_shaders.emplace_back("seabed.vert");
        seabed_shaders.emplace_back("seabed.frag");
        ShaderProgram seabed_shader_prog(seabed_shaders);

        // Load & create Perlin noise texture
        Texture2D perlin_tex = Texture2D("perlin_noise.jpg");

        // Create seabed renderer
        SeabedRenderer seabed_renderer(seabed_shader_prog, perlin_tex);


        // ------------------------------
        // Screen Quad (for visual debugging)

        // Create screen quad shaders
        std::vector<Shader> screen_quad_shaders;
        screen_quad_shaders.emplace_back("screen_quad.vert");
        screen_quad_shaders.emplace_back("screen_quad.frag");
        ShaderProgram screen_quad_shader_prog(screen_quad_shaders);

        // Create ocean renderer
        ScreenQuadRenderer screen_quad_renderer(screen_quad_shader_prog, ocean_renderer.getTextureS());
        
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


            // --- update water base colour params if changed in UI ---
            if (last_water_base_colour != m_context.m_water_base_colour)
            {
                ocean_renderer.setWaterBaseColour(m_context.m_water_base_colour);
                last_water_base_colour = m_context.m_water_base_colour;
            }

            if (last_water_base_colour_amt != m_context.m_water_base_colour_amt)
            {
                ocean_renderer.setWaterBaseColourAmount(m_context.m_water_base_colour_amt);
                last_water_base_colour_amt = m_context.m_water_base_colour_amt;
            }

            
            // --- render to texture S for refraction ---
            ocean_renderer.bindFBO();
            m_window.clear();

            // render seabed
            if (m_context.m_do_render_seabed)
            {
                seabed_renderer.render(m_context.m_render_camera);
            }
            // render skybox
            skybox_renderer.render(m_context.m_render_camera);

            // go back to default fbo
            ocean_renderer.unbindFBO();

            // --- render ocean ---
            if (m_context.m_do_render_ocean)
            {
                ocean_renderer.render(m_context.m_render_camera);
            }

            // --- render seabed ---
            if (m_context.m_do_render_seabed)
            {
                seabed_renderer.render(m_context.m_render_camera);
            }

            // --- render skybox ---
            skybox_renderer.render(m_context.m_render_camera);

            // --- render texture S for debugging ---
            //screen_quad_renderer.render(m_context.m_render_camera);

            // --- render UI ---
            UI::render();

            // --- draw on screen ---
            // flip front & back buffers; and draw
            m_window.update();

            glfwPollEvents();
        }
    }
}
