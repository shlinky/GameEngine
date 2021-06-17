#define GLEW_STATIC
//#define USING_MICROSOFT_BRKPTS
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <iomanip>
#include <string>
#include "OGLError.h"
#include "OGLShaderProgram.h"
#include "OGLVertexObject.h"
#include "WindowsWindowing.h"
#include "OGLTexturedShader.h"
#include "OGLFrameBuffer.h"
#include "Camera.h"
#include "ModelLoader.h"
#include <math.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/ext.hpp>
#include <GLM/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include "SceneMeshObject.h"
#include "EnvCube.h"
#include "Windows.h"
using namespace std;

//main file
//entry point in to application
//not part of engine (would be placed in the game project not engine project)
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

string cubeMapNames[6] = {
    "newgear0.jpg",
    "newgear1.jpg",
    "newgear2.jpg",
    "newgear3.jpg",
    "newgear4.jpg",
    "newgear5.jpg"
};


void keyInput(WindowsWindowing* w, SceneMeshObject* g, Camera* c) {
    glm::vec3 p = g->getPosition();
    glm::vec3 oc = c->getPosition();
    glm::vec3 dir = c->getForwardDir();
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
    if (w->isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT)) {
        cout << "left mouse pressed" << endl;
    }
    if (w->isMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT)) {
        cout << "right mouse pressed" << endl;
    }
}

void updateCameraAngle(double* cPos, double* lPos, SceneObject* g, Camera* c) {
    double dX = cPos[0] - lPos[0];
    double dY = -1 * (cPos[1] - lPos[1]);

    float y = c->getYaw();
    float p = c->getPitch();
    c->rotateYaw(dX);
    c->rotatePitch(dY);
    c->computeVectors();
    

    float yaw = c->getYaw();
    float ptch = c->getPitch();

    glm::vec3 cpos = c->getPosition();
    g->setPosition(cpos.x, cpos.y, cpos.z);
    g->setRotation(-90 - yaw, ptch, 0);
}

float screenvp[12] = {
    1, -1, 0,
    1,  1, 0,
   -1,  1, 0,
   -1, -1, 0,
};

unsigned int screenindex[6] = {
    0, 1, 2,
    0, 3, 2
};

int main(void)
{
    WindowsWindowing window(1920, 1080, "Application", false);

    startGLDebug();

    int sizex = window.getSizeX();
    int sizey = window.getSizeY();
    Camera cam(0.0f, 3.0f, 8.0f, sizex, sizey);
    cam.setSensitivity(0.05);
    cam.setPitchLimits(-87, 87);
    cam.setRotation(180, 0);

    OGLVertexObject model1("res/models/gucci.txt", true);

    SceneObject player = SceneObject();

    cout << "started1" << endl;

    OGLImageTexture brdff = OGLImageTexture("res/shaders/brdf.png");
    OGLCubeMapTexture cm = OGLCubeMapTexture("res/shaders/bob4.hdr", 512);
    OGLCubeMapTexture* irr = cm.createIrradianceMap(100);
    OGLCubeMapTexture* spec = cm.createPrefilteredSpec(100);
    EnvCube env = EnvCube(&cm);

    SceneMeshObject* sp[8] = { 0 };
    for (int i = 0; i < 5; i++) {
        sp[i] = new SceneMeshObject(&model1);
        sp[i]->setPosition(1 + (2 * i), 1, 1);
        sp[i]->createShader("res/shaders/b.vert", "res/shaders/pbr_mixed.frag", 6, 8);
        sp[i]->getShader()->addTexture("res/models/PreviewSphere" + to_string(i % 5) + "_Sphere_BaseColor.png");
        sp[i]->getShader()->addTexture("res/models/PreviewSphere" + to_string(i % 5) + "_Sphere_Normal.png");
        sp[i]->getShader()->addTexture("res/models/PreviewSphere" + to_string(i % 5) + "_Sphere_OcclusionRoughnessMetallic.png");
        sp[i]->getShader()->addTexture(irr);
        sp[i]->getShader()->addTexture(&brdff);
        sp[i]->getShader()->addTexture(spec);
        sp[i]->getShader()->addUniform<OGLUniform3FV>("colorId");

    }

    SceneMeshObject floor = SceneMeshObject("res/models/floor.txt");
    floor.createShader("res/shaders/b.vert", "res/shaders/pbr_mixed_tile.frag", 6, 8);
    floor.setScale(200, 200, 200);
    floor.getShader()->addTexture("res/models/plane_divided_DefaultMaterial_BaseColor.png");
    floor.getShader()->addTexture("res/models/plane_divided_DefaultMaterial_Normal.png");
    floor.getShader()->addTexture("res/models/plane_divided_DefaultMaterial_OcclusionRoughnessMetallic.png");
    floor.getShader()->addTexture(irr);
    floor.getShader()->addTexture(&brdff);
    floor.getShader()->addTexture(spec);
    floor.getShader()->addUniform<OGLUniform3FV>("colorId");

    SceneMeshObject knf = SceneMeshObject("res/models/knife.txt");
    knf.createShader("res/shaders/b.vert", "res/shaders/pbr_mixed.frag", 6, 8);
    knf.setPosition(-4, 0, 0);
    knf.setScale(2, 2, 2);
    knf.getShader()->addTexture("res/models/kcolor.png");
    knf.getShader()->addTexture("res/models/knorm.png");
    knf.getShader()->addTexture("res/models/korm.png");
    knf.getShader()->addTexture(irr);
    knf.getShader()->addTexture(&brdff);
    knf.getShader()->addTexture(spec);
    knf.getShader()->addUniform<OGLUniform3FV>("colorId");

    SceneMeshObject random = SceneMeshObject(0.45, -0.3, -2.3);
    random.setRotation(180, 0, 0);
    random.setScale(0.2, 0.2, 0.2);
    random.setIsComponent(true);
    random.setParent(&player);
    random.setMesh("res/models/test.txt");
    random.createShader("res/shaders/b.vert", "res/shaders/pgun_mixed.frag", 6, 6);
    random.getShader()->addTexture("res/models/v_portalgun.png");
    random.getShader()->addTexture("res/models/pgun_n.png");
    random.getShader()->addTexture("res/models/v_portalgun_s.jpg");
    random.getShader()->addTexture(irr);
    random.getShader()->addTexture(&brdff);
    random.getShader()->addTexture(spec);
    random.getShader()->addUniform<OGLUniform3FV>("colorId");

    sp[5] = &knf;
    sp[6] = &random;
    sp[7] = &floor;

    OGLFrameBuffer fb;
    OGLImageTexture colbb(sizex, sizey);
    fb.attachColorTexture(&colbb);
    fb.unbind();


    OGLTexturedShader postprocess = OGLTexturedShader("res/shaders/pp.vert", "res/shaders/pp.frag", 0, 3);
    postprocess.addTexture(&colbb);

    OGLVertexObject screenquad = OGLVertexObject(4);
    screenquad.addAttribute(0, 3, screenvp);
    screenquad.addIndexing(screenindex, 6);

    cam.setRotation(-70, -10);
    
    double lmpos[2] = {};
    double cmpos[2] = {};

    window.getMousePos(lmpos);

    window.prepareForNextFrame();
    /* Loop until the user closes the window */
    while (!window.isWindowClosing())
    {
        //cout << "started" << endl;
        window.getMousePos(cmpos);
        keyInput(&window, &random, &cam);
        updateCameraAngle(cmpos, lmpos, &player, &cam);
        
        lmpos[0] = cmpos[0];
        lmpos[1] = cmpos[1];

        //cout << glm::to_string(cam.getForwardDir()) << endl;
  
        //fb.unbind();
        //screenquad.bind();
        //postprocess.bindShaderProgram();
        //necol.bindTexture();
        //glBindTexture(GL_TEXTURE_2D, colbb.getId());
        //glDrawElements(GL_TRIANGLES, screenquad.getIndexCount(), GL_UNSIGNED_INT, (void*)0);


        for (int i = 0; i < 8; i++) {
            glm::vec3 c = glm::vec3((1.0f / 8.0f) * ((float)i + 1), 0, 0);
            sp[i]->getShader()->updateUniformData("colorId", &(c[0]));
            sp[i]->render(&cam);
        }
        
        window.prepareForNextFrame();

    }

    return 0;
}