#include "WindowsWindowing.h"

WindowsWindowing::WindowsWindowing(int sizex, int sizey, string title)
{
    this->sizex = sizex;
    this->sizey = sizey;

    /* Initialize the library */
    if (!glfwInit())
        applicationErrorCallback("Failed to init GLFW");

    /* Create a windowed mode window and its OpenGL context */
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // We want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(sizex, sizey, title.c_str(), NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        applicationErrorCallback("Failed to create window");
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    cout << ((glewInit() == GLEW_OK) ? "GLEW INITIALIZED" : "GLEW INIT FAILED") << endl;

    cout << glGetString(GL_VERSION) << endl;

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    glfwSwapInterval(1);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

WindowsWindowing::~WindowsWindowing()
{
    glfwTerminate();
}

bool WindowsWindowing::isWindowClosing()
{
    return glfwWindowShouldClose(window);
}

void WindowsWindowing::prepareForNextFrame()
{
    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
}

int WindowsWindowing::getSizeX()
{
    return sizex;
}

int WindowsWindowing::getSizeY()
{
    return sizey;
}

void WindowsWindowing::getMousePos(double* mPos)
{
    glfwGetCursorPos(window, &mPos[0], &mPos[1]);
}

void WindowsWindowing::setKeyPress(GLFWkeyfun K)
{
    glfwSetKeyCallback(window, K);
}

bool WindowsWindowing::isKeyPressed(int key)
{
    return glfwGetKey(window, key);
}
