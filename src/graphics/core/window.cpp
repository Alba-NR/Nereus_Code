
#include "window.h"

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
        fprintf(stderr, "Failed to Create GLFW Window");
        // TODO raise exception
    }

    // make the OpenGL context current & load OpenGL functions
    glfwMakeContextCurrent(m_window);
    gladLoadGL();
    fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));

    // --- set OpenGL viewport --- 
    // (OpenGL will render in this viewport)
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
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

// Set callback functions (not involving any camera state, mouse state nor any other state in nereus.h
void Window::setCallbacks()
{
    // Keyboard key press, repeat or release
    glfwSetKeyCallback(
        m_window,
        [](GLFWwindow *window, int key, int scancode, int action, int mods) 
        {
            // ESC released --> close window 
            if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
                glfwSetWindowShouldClose(window, true);
        }
    );
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