
#include "window.h"
#include "../main/app_context.h"
#include "../utils/image_io.h"

#include <iostream>

Window *Window::s_instance = nullptr;

Window &Window::getInstance()
{
    if (Window::s_instance == nullptr)
    {
        Window::s_instance = new Window();
    }

    return *Window::s_instance;
}

Window::Window()
{
    this->createWindow();
}

// Create a glfw window & set-up the appropriate context
void Window::createWindow()
{
    // do not attempt to re-init glfw if a window has already been created
    if (m_window != nullptr) return;

    // --- glfw window context ---
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, NereusConstants::OPENGL_VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, NereusConstants::OPENGL_VERSION_MINOR);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    // --- glfw window creation ---
    m_window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Nereus", nullptr, nullptr);

    // check for correct context creation
    if(m_window == nullptr)
    {
        glfwTerminate();
        std::cout << "ERROR::WINDOW::WINDOW_CREATION_FAILURE" << std::endl
            << "Failed to create GLFW window. Terminating GLFW." << std::endl;
        return;
    }

    // make the OpenGL context current & load OpenGL functions
    glfwMakeContextCurrent(m_window);
    gladLoadGL();
    fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));

    // --- set OpenGL viewport --- 
    // (OpenGL will render in this viewport)
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // --- enable depth test ---
    glEnable(GL_DEPTH_TEST);

    // --- enable back face culling ---
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    // --- set window icon ---
   
    GLFWimage images[1];
    int num_channels;
    // load image from file
    images[0].pixels = ImageIO::loadImage("nereus_logo/Nereus_Icon_Small_Sticker_Plain.png", images[0].width, images[0].height, num_channels, false);
    // set icon
    glfwSetWindowIcon(m_window, 1, images);
    // free img data
    stbi_image_free(images[0].pixels);
}

Window::~Window()
{
    this->close();
    Window::s_instance = nullptr;
}


// Initialises the ImGui GLFW backend (since must have access to GLFWwindow * to do so)
void Window::initImGuiGLFW()
{
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
}


// Swaps back & front buffers
void Window::update()
{
    glfwSwapBuffers(m_window);
}

void Window::close()
{
    glfwDestroyWindow(m_window);
}

// Clear the window to the background colour; also clear depth buffer
void Window::clear() {
    glClearColor(m_bgColour.x, m_bgColour.y, m_bgColour.z, 1.0f); // specify colour to clear to
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     // clear screen's color buffer & depth buffer
}

// Returns true if the glfw window should close
bool Window::shouldClose()
{
    return glfwWindowShouldClose(m_window);
}

// Set the glfw window should close flag to true
void Window::setShouldClose()
{
    return glfwSetWindowShouldClose(m_window, true);
}

// Set callback functions
void Window::setCallbacks()
{
    // Keyboard key press, repeat or release
    glfwSetKeyCallback(
        m_window,
        [](GLFWwindow *window, int key, int scancode, int action, int mods) 
        {
            Nereus::AppContext *context = reinterpret_cast<Nereus::AppContext *>(glfwGetWindowUserPointer(window));

            // ESC released --> close window 
            if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
            {
                glfwSetWindowShouldClose(window, true);
            }

            // U pressed --> toggle on/off rendering the UI
            if (key == GLFW_KEY_U && action == GLFW_RELEASE)
            {
                context->m_do_render_ui = !(context->m_do_render_ui);
            }
               
            // AWSD, left shift & left ctrl pressed --> move camera position
            // in main render loop (as has to be done when press & held)
        }
    );

    // Cursor position
    glfwSetCursorPosCallback(m_window,
        [](GLFWwindow *window, double xpos, double ypos)
        {
            Nereus::AppContext *context = reinterpret_cast<Nereus::AppContext *>(glfwGetWindowUserPointer(window));
            
            // click & drag --> move camera view
            bool dragging = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
            if (dragging) {
                float xoffset = (xpos - context->m_last_mouse_x);
                float yoffset = (context->m_last_mouse_y - ypos);
                context->m_render_camera.processMouseMovement(xoffset, yoffset);
            }

            context->m_last_mouse_x = xpos;
            context->m_last_mouse_y = ypos;
        }
    );

    // Cursor scroll
    glfwSetScrollCallback(m_window,
        [](GLFWwindow *window, double xoffset, double yoffset)
        {
            Nereus::AppContext *context = reinterpret_cast<Nereus::AppContext *>(glfwGetWindowUserPointer(window));

            // scroll --> zoom in/out
            context->m_render_camera.processMouseScroll(yoffset);
        }
    );
}

// Sets the GLFW window's window user pointer to the given pointer
void Window::setWindowUserPointer(void *ptr)
{
    glfwSetWindowUserPointer(m_window, ptr);
}


// Return the state of the given glfw keyboard key
unsigned int Window::getKeyState(unsigned int key)
{
    return glfwGetKey(m_window, key);
}

// Get the SCREEN_WIDTH value
unsigned int Window::getScreenWidth()
{
    return SCREEN_WIDTH;
}

// Get the SCREEN_HEIGHT value
unsigned int Window::getScreenHeight()
{
    return SCREEN_HEIGHT;
}
