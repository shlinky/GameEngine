#define GLEW_STATIC
#define USING_MICROSOFT_BRKPTS
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <iomanip>
#include <string>
#define DEBUG
#include "OGLError.h"
#include "OGLShaderProgram.h"
#include "OGLVertexObject.h"
#include "WindowsWindowing.h"
#include "OGLTexturedShader.h"
#include "Camera.h"
#include "ModelLoader.h"
#include <math.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/ext.hpp>
#include <GLM/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
using namespace std;

//add scenemesh class
//add scene class
//HASH connecting names to objects
//multiple cameras if only one select that one
//lights
//map class
//portals
//collisions
//animations and skeletal meshes bones
//blurred map
//lihgts as scene objects
//collisions
//physics
//ray casting (path tracing)
//ui through imgui

float lightPosition[3] = {
    1.0f,
    2.0f,
    4.0f
};

float lightColor[3] = {
    1.0f,
    1.0f,
    1.0f
};

string cubeMapbadNames[6] = {
    "res/Storforsen3/posx.jpg",
    "res/Storforsen3/negx.jpg",
    "res/Storforsen3/posy.jpg",
    "res/Storforsen3/negy.jpg",
    "res/Storforsen3/posz.jpg",
    "res/Storforsen3/negz.jpg"
};

string cubeMapNames[6] = {
    "res/skybox1/skybox/right.jpg",
    "res/skybox1/skybox/left.jpg",
    "res/skybox1/skybox/top.jpg",
    "res/skybox1/skybox/bottom.jpg",
    "res/skybox1/skybox/front.jpg",
    "res/skybox1/skybox/back.jpg"
};


void keyInput(WindowsWindowing* w, Camera* c) {
    if (w->isKeyPressed(GLFW_KEY_W))
        c->moveForward(0.5);
    if (w->isKeyPressed(GLFW_KEY_A))
        c->moveRight(-0.5);
    if (w->isKeyPressed(GLFW_KEY_S))
        c->moveForward(-0.5);
    if (w->isKeyPressed(GLFW_KEY_D))
        c->moveRight(0.5);
    if (w->isKeyPressed(GLFW_KEY_SPACE))
        c->moveUp(0.5);
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
    WindowsWindowing window(1920, 1080, "Application", false);

    startGLDebug();

    int sizex = window.getSizeX();
    int sizey = window.getSizeY();
    Camera cam(0.0f, 0.0f, 0.0f, &sizex, &sizey);
    cam.setSensitivity(0.05);
    cam.setPitchLimits(-87, 87);

    OGLVertexObject model("res/models/test.txt", true);
    OGLVertexObject model1("res/models/gucci.txt", true);
    OGLVertexObject envcube("res/models/basic.obj");

    OGLTexturedShader shaderp("res/shaders/b.vert", "res/shaders/dir1.frag", 5, 4);
    shaderp.addUniform<OGLUniformMat4FV>("coolbeans");
    shaderp.addUniform<OGLUniformMat4FV>("world");
    shaderp.addUniform<OGLUniform3FV>("light_position", &lightPosition);
    shaderp.addUniform<OGLUniform3FV>("light_color", &lightColor);
    shaderp.addUniform<OGLUniform3FV>("camera_position");
    shaderp.addCubemapTexture(cubeMapNames);
    shaderp.addTexture("res/models/kcolor.png");
    shaderp.addTexture("res/models/knorm.png");
    shaderp.addTexture("res/models/korm.png");

    OGLTexturedShader* sp[4] = { 0 };
    for (int i = 0; i < 4; i++) {
        sp[i] = new OGLTexturedShader("res/shaders/b.vert", "res/shaders/metallic_sphere.frag", 5, 4);
        sp[i]->addUniform<OGLUniformMat4FV>("coolbeans");
        sp[i]->addUniform<OGLUniformMat4FV>("world");
        sp[i]->addUniform<OGLUniform3FV>("light_position", &lightPosition);
        sp[i]->addUniform<OGLUniform3FV>("light_color", &lightColor);
        sp[i]->addUniform<OGLUniform3FV>("camera_position");
        sp[i]->addCubemapTexture(cubeMapNames);
        sp[i]->addTexture("res/models/PreviewSphere" + to_string(i) + "_Sphere_BaseColor.png");
        sp[i]->addTexture("res/models/PreviewSphere" + to_string(i) + "_Sphere_Normal.png");
        sp[i]->addTexture("res/models/PreviewSphere" + to_string(i) + "_Sphere_OcclusionRoughnessMetallic.png");
    }
    //shaderp.addTexture("res/shaders/cloth_seat.jpg");

    OGLTexturedShader shaderpc("res/shaders/bc.vert", "res/shaders/wcube.frag", 4, 1);
    shaderpc.addUniform<OGLUniformMat4FV>("coolbeans");
    shaderpc.addUniform<OGLUniform3FV>("light_position", &lightPosition);
    shaderpc.addUniform<OGLUniform3FV>("light_color", &lightColor);
    shaderpc.addUniform<OGLUniform3FV>("camera_position");
    shaderpc.addCubemapTexture(cubeMapNames);

    double lmpos[2] = {};
    double cmpos[2] = {};

    window.getMousePos(lmpos);
    
    window.prepareForNextFrame();
    /* Loop until the user closes the window */
    while (!window.isWindowClosing())
    {
        window.getMousePos(cmpos);
        updateCameraAngle(cmpos, lmpos, &cam);
        keyInput(&window, &cam);
        
        lmpos[0] = cmpos[0];
        lmpos[1] = cmpos[1];

        float campos[3]; 
        cam.getPosition(campos);
        shaderp.updateUniformData("camera_position", (void*)&campos);
        shaderpc.updateUniformData("camera_position", (void*)&campos);

        glm::mat4 v = glm::make_mat4(cam.getTransMat());
        glm::mat4 mvp = v;
        glm::mat4 i = glm::identity<glm::mat4>();
        shaderp.updateUniformData("coolbeans", &mvp);
        shaderp.updateUniformData("world", &i);
        mvp = v * glm::translate(glm::vec3(1, 1, 1));

        for (int i = 0; i < 4; i++) {
            mvp = v * glm::translate(glm::vec3(1 + (2 * i), 1, 1));
            glm::mat4 trans1 = glm::translate(glm::vec3(1 + (2 * i), 1, 1));

            sp[i]->updateUniformData("camera_position", (void*)&campos);
            sp[i]->updateUniformData("coolbeans", &mvp);
            sp[i]->updateUniformData("world", &trans1);
        }

        v = glm::make_mat4(cam.getTransMat(false));
        glm::mat4 mvpc = v * glm::scale(glm::identity<glm::mat4>(), glm::vec3(100, 100, 100));
        shaderpc.updateUniformData("coolbeans", &mvpc);
        
        //make a render function in env cube class
        //bind and render function in scene objects; render calls bind
        /**glDepthMask(GL_FALSE);
        envcube.bind();
        shaderpc.bindShaderProgram();
        glDrawElements(GL_TRIANGLES, envcube.getVertexCount(), GL_UNSIGNED_INT, (void*)0);
        glDepthMask(GL_TRUE);**/

        model.bind();
        shaderp.bindShaderProgram();        
        //glDrawElements(GL_TRIANGLES, model.getVertexCount(), GL_UNSIGNED_INT, (void*)0);

        for (int i = 0; i < 4; i++) {
            model1.bind();
            sp[i]->bindShaderProgram();
            glDrawElements(GL_TRIANGLES, model1.getVertexCount(), GL_UNSIGNED_INT, (void*)0);
        }

        window.prepareForNextFrame();
    }

    return 0;
}