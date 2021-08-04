#include "WindowsWindowing.h"

//add a delta mouse function with a sensitivity option
//set antialiasing

float WindowsWindowing::presstime = 0;
bool WindowsWindowing::clicked = false;

WindowsWindowing::WindowsWindowing(int sizex, int sizey, string title, bool fullscreen)
{
    this->sizex = sizex;
    this->sizey = sizey;

    /* Initialize the library */
    if (!glfwInit())
        applicationErrorCallback("Failed to init GLFW");
    else {
        cout << "windowing succeeded" << endl;
    }

    /* Create a windowed mode window and its OpenGL context */
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // We want OpenGL 3.3
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    if (fullscreen)
        window = glfwCreateWindow(sizex, sizey, title.c_str(), glfwGetPrimaryMonitor(), NULL);
    else
        window = glfwCreateWindow(sizex, sizey, title.c_str(), NULL, NULL);
    if (!window)
    {
        applicationErrorCallback("Failed to create window");
        glfwTerminate();
    }
    else {
        cout << "window creation succeeded" << endl;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    int gec = glewInit();
    cout << ((gec == GLEW_OK) ? "GLEW INITIALIZED" : "GLEW INIT FAILED") << endl;
    if (gec) cout << "error code: " << gec << endl;
    if (gec) cout << glewGetErrorString(gec) << endl;
    cout << glGetString(GL_VERSION) << endl;

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    glfwSwapInterval(1);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetMouseButtonCallback(window, WindowsWindowing::mouse_button_callback);
    clicked = false;
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
    glfwSwapBuffers(window);

    glfwPollEvents();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

bool WindowsWindowing::isMouseButtonPressed(int mouseButton)
{
    int state = glfwGetMouseButton(window, mouseButton);
    if (state == GLFW_PRESS)
    {
        return (true);
    }
    return (false);
}

bool WindowsWindowing::isMouseClicked()
{
    bool cclicked = clicked;
    clicked = false;
    return cclicked;
}

void WindowsWindowing::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        presstime = getTime();
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        float dtime = getTime() - presstime;
        if (dtime < 0.2) {
            clicked = true;
        }
        else {
            clicked = false;
        }
    }

}

void WindowsWindowing::setMouseHidden(bool hidden)
{
    glfwSetInputMode(window, GLFW_CURSOR, hidden ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

float WindowsWindowing::getTime()
{
    return glfwGetTime();
}

void WindowsWindowing::resetTime()
{
    glfwSetTime(0);
}
