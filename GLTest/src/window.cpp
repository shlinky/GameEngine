#define GLEW_STATIC
//#define DEBUG
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "OGLError.h"
#include "OGLShaderProgram.h"
#include "OGLVertexObject.h"
#include "WindowsWindowing.h"
#include "OGLImageTexture.h"
#include "Camera.h"
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/ext.hpp>
#include <GLM/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace std;

float vertices[72] = {
       -1.0f, -1.0f,  1.0f, //0
        1.0f, -1.0f,  1.0f, //1
        1.0f,  1.0f,  1.0f,
       -1.0f,  1.0f,  1.0f,
       -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
       -1.0f,  1.0f, -1.0f,
//second set
       -1.0f, -1.0f,  1.0f, //8
        1.0f, -1.0f,  1.0f, //9
        1.0f,  1.0f,  1.0f,
       -1.0f,  1.0f,  1.0f,
       -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
       -1.0f,  1.0f, -1.0f,
//third set
       -1.0f, -1.0f,  1.0f, //16
        1.0f, -1.0f,  1.0f, //17
        1.0f,  1.0f,  1.0f,
       -1.0f,  1.0f,  1.0f,
       -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
       -1.0f,  1.0f, -1.0f,
};

float normals[72] = {
        0.0f,  0.0f,  1.0f, //0
        0.0f,  0.0f,  1.0f, //1
        0.0f,  0.0f,  1.0f,
        0.0f,  0.0f,  1.0f,
        0.0f,  0.0f, -1.0f,
        0.0f,  0.0f, -1.0f,
        0.0f,  0.0f, -1.0f,
        0.0f,  0.0f, -1.0f,
//second set
       -1.0f,  0.0f,  0.0f, //8
        1.0f,  0.0f,  0.0f, //9
        1.0f,  0.0f,  0.0f,
       -1.0f,  0.0f,  0.0f,
       -1.0f,  0.0f,  0.0f,
        1.0f,  0.0f,  0.0f,
        1.0f,  0.0f,  0.0f,
       -1.0f,  0.0f,  0.0f,
//third set
        0.0f, -1.0f,  0.0f, //16
        0.0f, -1.0f,  0.0f, //17
        0.0f,  1.0f,  0.0f,
        0.0f,  1.0f,  0.0f,
        0.0f, -1.0f,  0.0f,
        0.0f, -1.0f,  0.0f,
        0.0f,  1.0f,  0.0f,
        0.0f,  1.0f,  0.0f,
};

float UV[48] = {
        0.0f,  0.0f,//0
        1.0f,  0.0f,//1
        1.0f,  1.0f,
        0.0f,  1.0f,
        0.0f,  0.0f,
        1.0f,  0.0f,
        1.0f,  1.0f,
        0.0f,  1.0f,
//second set
        0.0f,  0.0f,//0
        1.0f,  0.0f,//1
        1.0f,  1.0f,
        0.0f,  1.0f,
        0.0f,  0.0f,
        1.0f,  0.0f,
        1.0f,  1.0f,
        0.0f,  1.0f,
//third set
        0.0f,  0.0f,//0
        1.0f,  0.0f,//1
        1.0f,  1.0f,
        0.0f,  1.0f,
        0.0f,  0.0f,
        1.0f,  0.0f,
        1.0f,  1.0f,
        0.0f,  1.0f,
};


unsigned int vertOrder[36] = {
    0, 1, 2,
    0, 3, 2,
    4, 5, 6,
    4, 7, 6,
    8, 11, 15,
    8, 12, 15,
    9, 13, 14,
    9, 10, 14,
    16, 17, 21,
    16, 20, 21,
    18, 19, 23,
    18, 22, 23
};

float lightPosition[3] = {
    0.0f,
    3.0f,
    0.0f
};

float lightColor[3] = {
    1.0f,
    1.0f,
    1.0f
};

float cameraPosition[3] = {
    0.0f,
    0.0f,
    6.0f
};

void keyInput(WindowsWindowing* w, Camera* c) {
    if (w->isKeyPressed(GLFW_KEY_W))
        c->moveForward(0.1);
    if (w->isKeyPressed(GLFW_KEY_A))
        c->moveRight(-0.1);
    if (w->isKeyPressed(GLFW_KEY_S))
        c->moveForward(-0.1);
    if (w->isKeyPressed(GLFW_KEY_D))
        c->moveRight(0.1);
}

void updateCameraAngle(double* cPos, double* lPos, Camera* c) {
    double dX = cPos[0] - lPos[0];
    double dY = -1 * (cPos[1] - lPos[1]);

    c->rotateYaw(dX);
    c->rotatePitch(dY);
    c->computeVectors();
}

int main(void)
{
    WindowsWindowing window(1920, 1080, "Application");

    startGLDebug();

    Camera cam(0, 0, 3);
    cam.setSensitivity(0.05);
    cam.setPitchLimits(-90, 90);

    OGLVertexObject model(24, 3);
    model.addAttribute(0, 3, vertices);
    model.addAttribute(1, 3, normals);
    model.addAttribute(2, 2, UV);
    model.addIndexing(vertOrder, 36);
    model.bind();

    OGLShaderProgram shaderp("res/shaders/b.vert", "res/shaders/b.frag", 4);
    shaderp.addUniform<OGLUniformMat4FV>("coolbeans");
    shaderp.addUniform<OGLUniform3FV>("light_position", &lightPosition);
    shaderp.addUniform<OGLUniform3FV>("light_color", &lightColor);
    shaderp.addUniform<OGLUniform3FV>("camera_position", &cameraPosition);
    shaderp.bindShaderProgram();

    OGLImageTexture texture("res/shaders/cloth_seat.jpg");
    texture.bindTexture(0);

    double lmpos[2] = {};
    double cmpos[2] = {};

    glm::mat4 p = glm::perspective<float>(glm::radians(30.0f), (float)window.getSizeX() / window.getSizeY(), 0.1f, 1000.f);
    window.getMousePos(lmpos);
    
    /* Loop until the user closes the window */
    while (!window.isWindowClosing())
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        window.getMousePos(cmpos);
        updateCameraAngle(cmpos, lmpos, &cam);
        keyInput(&window, &cam);
        
        lmpos[0] = cmpos[0];
        lmpos[1] = cmpos[1];

        float campos[3]; 
        cam.getPosition(campos);
        shaderp.updateUniformData("camera_position", (void*)&campos);

        glm::mat4 v = glm::make_mat4(cam.getViewMat());
        glm::mat4 mvp = p * v * glm::identity<glm::mat4>();
        shaderp.updateUniformData("coolbeans", &mvp);
        shaderp.bindShaderProgram();

        
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)0);

        window.prepareForNextFrame();
    }

    return 0;
}