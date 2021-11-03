
#include "window.h"

Window *Window::instance = nullptr;

Window &Window::getInstance()
{
    if (Window::instance == nullptr)
    {
        Window::instance = new Window();
    }

    return *Window::instance;
}

Window::Window()
{
    this->createWindow();
}

// Create a glfw window & set-up the appropriate context
void Window::createWindow()
{
    // do not attempt to re-init glfw if a window has already been created
    if (window != nullptr) return;

    // --- glfw window context ---
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    // --- glfw window creation ---
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Nereus", nullptr, nullptr);

    // check for correct context creation
    if(window == nullptr)
    {
        glfwTerminate();
        fprintf(stderr, "Failed to Create GLFW Window");
        // TODO raise exception
    }

    // make the OpenGL context current & load OpenGL functions
    glfwMakeContextCurrent(this->window);
    gladLoadGL();
    fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));

    // --- set OpenGL viewport --- 
    // (OpenGL will render in this viewport)
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}

Window::~Window()
{
    this->close();
    Window::instance = nullptr;
}


// Swaps back & front buffers
void Window::update()
{
    glfwSwapBuffers(this->window);    
}

void Window::close()
{
    glfwDestroyWindow(this->window);
}

// Clear the window to the background colour; also clear depth buffer
void Window::clear() {
    glClearColor(this->bgColour.x, this->bgColour.y, this->bgColour.z, 1.0f); // specify colour to clear to
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     // clear screen's color buffer & depth buffer
}

// Returns true if the glfw window should close
bool Window::shouldClose()
{
    return glfwWindowShouldClose(this->window);
}

// Set the glfw window should close flag to true
void Window::setShouldClose()
{
    return glfwSetWindowShouldClose(this->window, true);
}

// Set callback functions (not involving any camera state, mouse state nor any other state in nereus.h
void Window::setCallbacks()
{
    // Keyboard key press, repeat or release
    glfwSetKeyCallback(
        this->window,
        [](GLFWwindow *window, int key, int scancode, int action, int mods) 
        {
            // ESC released --> close window 
            if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
                glfwSetWindowShouldClose(window, true);
            // W pressed --> view in wireframe mode
            if (key == GLFW_KEY_W) {
                if (action == GLFW_PRESS) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                else if (action == GLFW_RELEASE) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
        }
    );
}

// Return the state of the given glfw keyboard key
unsigned int Window::getKeyState(unsigned int key)
{
    return glfwGetKey(this->window, key);
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