
#include "window_manager.h"

namespace WindowManager
{
    // Create a glfw window & set-up the appropriate context
    int createWindow()
    {
        // do not attempt to re-init glfw if a window has already been created
        if (window != nullptr)
        {
            return EXIT_SUCCESS;
        }

        // --- glfw window context ---
        glfwInit();
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
            return EXIT_FAILURE;
        }

        // make the OpenGL context current & load OpenGL functions
        glfwMakeContextCurrent(WindowManager::window);
        gladLoadGL();
        fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));

        // --- set OpenGL viewport --- 
        // (OpenGL will render in this viewport)
        glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

        return EXIT_SUCCESS;
    }


    // Swaps back & front buffers
    void updateWindow()
    {
        glfwSwapBuffers(window);    
    }

    // Clear the window to the background colour; also clear depth buffer
    void clearWindow() {
        glClearColor(bgColour.x, bgColour.y, bgColour.z, 1.0f); // specify colour to clear to
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     // clear screen's color buffer & depth buffer
    }

    // Destroys the glfw window
    void closeWindow()
    {
        glfwDestroyWindow(window);
    }

    // Returns true if the glfw window should close
    bool windowShouldClose()
    {
        return glfwWindowShouldClose(window);
    }

    // Set the glfw window should close flag to true
    void setWindowShouldClose()
    {
        return glfwSetWindowShouldClose(window, true);
    }

    // Set callback functions (not involving any camera state, mouse state nor any other state in nereus.h
    void setCallbacks()
    {
        // Keyboard key press, repeat or release
        glfwSetKeyCallback(
            window, 
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
    unsigned int getKeyState(unsigned int key)
    {
        return glfwGetKey(window, key);
    }

    // Get the SCREEN_WIDTH value
    unsigned int getScreenWidth()
    {
        return SCREEN_WIDTH;
    }

    // Get the SCREEN_HEIGHT value
    unsigned int getScreenHeight()
    {
        return SCREEN_HEIGHT;
    }

}