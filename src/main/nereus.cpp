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
        string folder_names[] = { "sky_skybox_1", "sky_skybox_2", "sunset_skybox_1", "sunset_skybox_2", "sunset_skybox_3" };
        std::shared_ptr<CubeMapTexture> env_maps[] = {nullptr, nullptr, nullptr, nullptr, nullptr };
        for (int i = 0; i < 5; i++)
        {
            string folder_name = folder_names[i];
            string env_map_imgs[] = {
                folder_name + "/right.jpg",
                folder_name + "/left.jpg",
                folder_name + "/top.jpg",
                folder_name + "/bottom.jpg",
                folder_name + "/front.jpg",
                folder_name + "/back.jpg"
            };

            env_maps[i] = std::make_shared<CubeMapTexture>(env_map_imgs);
        }

        // Track last env map used
        int last_env_map = NereusConstants::DEFAULT_ENV_MAP;

        // Create Skybox renderer
        SkyBoxRenderer skybox_renderer(skybox_shader_prog, *env_maps[last_env_map]);


        // ------------------------------
        // Ocean

        // --- Create Mesh
        std::shared_ptr<GridMesh> ocean_mesh_ptr = std::make_shared<GridMesh>(NereusConstants::DEFAULT_OCEAN_GRID_WIDTH, NereusConstants::DEFAULT_OCEAN_GRID_LENGTH);
        ocean_mesh_ptr->initialise();

        // --- Fresnel
        // Create ocean surface shaders
        std::vector<Shader> ocean_shaders_fresnel;
        ocean_shaders_fresnel.emplace_back("ocean_wavesim.vert");
        ocean_shaders_fresnel.emplace_back("ocean_fresnel.frag");
        ShaderProgram ocean_shader_prog_fresnel(ocean_shaders_fresnel);
        // Create ocean renderer
        FullOceanRenderer ocean_renderer_fresnel(ocean_shader_prog_fresnel, ocean_mesh_ptr, skybox_renderer.getCubeMapTexture());

        // --- Reflection
        std::vector<Shader> ocean_shaders_refl;
        ocean_shaders_refl.emplace_back("ocean_wavesim.vert");
        ocean_shaders_refl.emplace_back("ocean_refl.frag");
        ShaderProgram ocean_shader_prog_refl(ocean_shaders_refl);
        ReflectiveOceanRenderer ocean_renderer_refl(ocean_shader_prog_refl, ocean_mesh_ptr, skybox_renderer.getCubeMapTexture());

        // --- Refraction
        std::vector<Shader> ocean_shaders_refr;
        ocean_shaders_refr.emplace_back("ocean_wavesim.vert");
        ocean_shaders_refr.emplace_back("ocean_refr.frag");
        ShaderProgram ocean_shader_prog_refr(ocean_shaders_refr);
        RefractiveOceanRenderer ocean_renderer_refr(ocean_shader_prog_refr, ocean_mesh_ptr);

        // --- Phong
        std::vector<Shader> ocean_shaders_phong;
        ocean_shaders_phong.emplace_back("ocean_wavesim.vert");
        ocean_shaders_phong.emplace_back("ocean_phong.frag");
        ShaderProgram ocean_shader_prog_phong(ocean_shaders_phong);
        OceanRenderer ocean_renderer_phong(ocean_shader_prog_phong, ocean_mesh_ptr);


        // --- Other params
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

        // Load & create Seabed textures
        string seabed_imgs_names[] = { "sand_seabed_1.jpg", "sand_seabed_2.jpg", "petrified_seabed.jpg" };
        std::shared_ptr<Texture2D> seabed_textures[] = { nullptr, nullptr, nullptr };
        for (int i = 0; i < 3; i++)
        {
            seabed_textures[i] = std::make_shared<Texture2D>(seabed_imgs_names[i]);
        }


        // Create seabed renderer
        SeabedRenderer seabed_renderer(seabed_shader_prog, perlin_tex);

        // Track last SEABED size values
        int last_seabed_mesh_grid_width = NereusConstants::DEFAULT_SEABED_GRID_WIDTH;
        int last_seabed_mesh_grid_length = NereusConstants::DEFAULT_SEABED_GRID_LENGTH;

        // Track last seabed texture used
        int last_seabed_tex = 0; // none

        
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
                ocean_renderer_fresnel.updateOceanMeshGrid(m_context.m_ocean_grid_width, m_context.m_ocean_grid_length);
                m_context.m_num_ocean_primitives = 2 * m_context.m_ocean_grid_width * m_context.m_ocean_grid_length;
                last_ocean_mesh_grid_width = m_context.m_ocean_grid_width;
                last_ocean_mesh_grid_length = m_context.m_ocean_grid_length;
            }

            if (last_seabed_mesh_grid_width != m_context.m_seabed_grid_width
                || last_seabed_mesh_grid_length != m_context.m_seabed_grid_length)
            {
                seabed_renderer.updateSeabedMeshGrid(m_context.m_seabed_grid_width, m_context.m_seabed_grid_length);
                m_context.m_num_seabed_primitives = 2 * m_context.m_seabed_grid_width * m_context.m_seabed_grid_length;
                last_seabed_mesh_grid_width = m_context.m_seabed_grid_width;
                last_seabed_mesh_grid_length = m_context.m_seabed_grid_length;
            }

            // update ocean size info if the size has been changed in the UI
            if (last_ocean_width != m_context.m_ocean_width)
            {
                ocean_renderer_fresnel.setOceanWidth(m_context.m_ocean_width);
                ocean_renderer_refl.setOceanWidth(m_context.m_ocean_width);
                ocean_renderer_refr.setOceanWidth(m_context.m_ocean_width);
                ocean_renderer_phong.setOceanWidth(m_context.m_ocean_width);
                seabed_renderer.setSeabedWidth(m_context.m_ocean_width + NereusConstants::SEABED_EXTENSION_FROM_OCEAN);
                last_ocean_width = m_context.m_ocean_width;
            }
            if (last_ocean_length != m_context.m_ocean_length)
            {
                ocean_renderer_fresnel.setOceanLength(m_context.m_ocean_length);
                ocean_renderer_refl.setOceanLength(m_context.m_ocean_length);
                ocean_renderer_refr.setOceanLength(m_context.m_ocean_length);
                ocean_renderer_phong.setOceanLength(m_context.m_ocean_length);
                seabed_renderer.setSeabedLength(m_context.m_ocean_length + NereusConstants::SEABED_EXTENSION_FROM_OCEAN);
                last_ocean_length = m_context.m_ocean_length;
            }


            // --- update water base colour params if changed in UI ---
            if (last_water_base_colour != m_context.m_water_base_colour)
            {
                ocean_renderer_fresnel.setWaterBaseColour(m_context.m_water_base_colour);
                ocean_renderer_refl.setWaterBaseColour(m_context.m_water_base_colour);
                ocean_renderer_refr.setWaterBaseColour(m_context.m_water_base_colour);
                ocean_renderer_phong.setWaterBaseColour(m_context.m_water_base_colour);
                last_water_base_colour = m_context.m_water_base_colour;
            }

            if (last_water_base_colour_amt != m_context.m_water_base_colour_amt)
            {
                ocean_renderer_fresnel.setWaterBaseColourAmount(m_context.m_water_base_colour_amt);
                ocean_renderer_refl.setWaterBaseColourAmount(m_context.m_water_base_colour_amt);
                ocean_renderer_refr.setWaterBaseColourAmount(m_context.m_water_base_colour_amt);
                last_water_base_colour_amt = m_context.m_water_base_colour_amt;
            }


            // --- update env map used if changed in UI
            if (last_env_map != m_context.m_env_map)
            {
                skybox_renderer.setCubeMapTexture(*env_maps[m_context.m_env_map]);
                ocean_renderer_fresnel.setSkyboxTexture(*env_maps[m_context.m_env_map]);
                ocean_renderer_refl.setSkyboxTexture(*env_maps[m_context.m_env_map]);
                last_env_map = m_context.m_env_map;
            }

            // --- update seabed texture used if changed in UI
            if (last_seabed_tex != m_context.m_seabed_tex)
            {
                if (m_context.m_seabed_tex == 0)
                {
                    seabed_renderer.removeSeabedTexture();
                }
                else
                {
                    seabed_renderer.setSeabedTexture(*seabed_textures[m_context.m_seabed_tex-1]);
                }
                last_seabed_tex = m_context.m_seabed_tex;
            }
            

            // --- render to texture S for refraction ---
            if (m_context.m_illumin_model % 2 == 0)
            {
                if (m_context.m_illumin_model == 0)
                {
                    ocean_renderer_fresnel.bindFBO();
                } else
                {
                    ocean_renderer_refr.bindFBO();
                }

                m_window.clear();

                // render seabed
                if (m_context.m_do_render_seabed)
                {
                    seabed_renderer.render(m_context.m_render_camera);
                }
                // render skybox
                skybox_renderer.render(m_context.m_render_camera);

                // go back to default fbo
                if (m_context.m_illumin_model == 0)
                {
                    ocean_renderer_fresnel.unbindFBO();
                }
                else
                {
                    ocean_renderer_refr.unbindFBO();
                }
            }
            

            // --- render ocean ---
            if (m_context.m_do_render_ocean)
            {
                switch (m_context.m_illumin_model)
                {
                case 0:
                    ocean_renderer_fresnel.render(m_context.m_render_camera);
                    break;
                case 1:
                    ocean_renderer_refl.render(m_context.m_render_camera);
                    break;
                case 2:
                    ocean_renderer_refr.render(m_context.m_render_camera);
                    break;
                default:
                    ocean_renderer_phong.render(m_context.m_render_camera);
                    break;
                }
            }

            // --- render seabed ---
            if (m_context.m_do_render_seabed)
            {
                seabed_renderer.render(m_context.m_render_camera);
            }

            // --- render skybox ---
            skybox_renderer.render(m_context.m_render_camera);

            // --- render UI ---
            if (m_context.m_do_render_ui)
            {
                UI::render();
            }

            // --- draw on screen ---
            // flip front & back buffers; and draw
            m_window.update();

            glfwPollEvents();
        }
    }
}
