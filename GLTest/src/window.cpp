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

#include "Scene.h"
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


bool keyInput(WindowsWindowing* w, SceneMeshObject* g, Camera* c) {
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
    if ((w->isKeyPressed(GLFW_KEY_SPACE)) && (!w->isKeyPressed(GLFW_KEY_RIGHT_SHIFT)))
        c->moveUp(0.5);
    if ((w->isKeyPressed(GLFW_KEY_SPACE)) && (w->isKeyPressed(GLFW_KEY_RIGHT_SHIFT)))
        c->moveUp(-0.5);
    if (w->isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT)) {
        return true;
    }
    else {
        return false;
    }
}

void updateCameraAngle(WindowsWindowing* w, double* cPos, double* lPos, SceneObject* g, Camera* c) {
    if (w->isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT)) {
        w->setMouseHidden(true);
        double dX = cPos[0] - lPos[0];
        double dY = -1 * (cPos[1] - lPos[1]);

        float y = c->getYaw();
        float p = c->getPitch();
        c->rotateYaw(dX);
        c->rotatePitch(dY);
        c->computeVectors();


        float yaw = c->getYaw();
        float ptch = c->getPitch();
        //cout << yaw << endl;
        g->setRotation(ptch, -90 - yaw, 0);
    }
    else {
        w->setMouseHidden(false);
    }
    glm::vec3 cpos = c->getPosition();
    g->setPosition(cpos.x, cpos.y, cpos.z);
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
    WindowsWindowing window(640, 480, "Application", false);

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
    OGLCubeMapTexture cm = OGLCubeMapTexture("res/shaders/bob.hdr", 512);
    OGLCubeMapTexture* irr = cm.createIrradianceMap(256);
    OGLCubeMapTexture* spec = cm.createPrefilteredSpec(256);
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
    knf.setPosition(3.2, -4.5, -2.8);
    knf.setScale(1, 1, 1);
    knf.setRotation(90, -88, 0);
    knf.setIsComponent(true);
    knf.setParent(&player);
    knf.getShader()->addTexture("res/models/revolv1_low_Material_BaseColor.png");
    knf.getShader()->addTexture("res/models/revolv1_low_Material_Normal.png");
    knf.getShader()->addTexture("res/models/revolv1_low_Material_OcclusionRoughnessMetallic.png");
    knf.getShader()->addTexture(irr);
    knf.getShader()->addTexture(&brdff);
    knf.getShader()->addTexture(spec);
    knf.getShader()->addUniform<OGLUniform3FV>("colorId");

    SceneMeshObject random = SceneMeshObject(0.45, -0.3, -2.3);
    random.setRotation(0, 180, 0);
    random.setScale(0.2, 0.2, 0.2);
    //random.setIsComponent(true);
    //random.setParent(&player);
    random.setMesh("res/models/test.txt");
    random.createShader("res/shaders/b.vert", "res/shaders/pgun_mixed.frag", 6, 6);
    random.getShader()->addTexture("res/models/v_portalgun.png");
    random.getShader()->addTexture("res/models/pgun_n.png");
    random.getShader()->addTexture("res/models/v_portalgun_s.jpg");
    random.getShader()->addTexture(irr);
    random.getShader()->addTexture(&brdff);
    random.getShader()->addTexture(spec);
    random.getShader()->addUniform<OGLUniform3FV>("colorId");

    sp[5] = &floor;
    sp[6] = &random;
    sp[7] = &knf;





    //OGLImageTexture necol(colbb.getWidth(), colbb.getHeight(), pixels);
    double lmpos[2] = {};
    double cmpos[2] = {};

    window.getMousePos(lmpos);

    window.prepareForNextFrame();

    bool was_pressed = false;

    Scene scn = Scene(&window);
    for (int i = 0; i < 8; i++) {
        scn.addSceneObject(sp[i]);
    }
    //scn.addSceneObject(&env);
    scn.setEditorFunctionality(true);
    // Loop until the user closes the window 
    while (!window.isWindowClosing())
    {
        //cout << "started" << endl;
        window.getMousePos(cmpos);
        bool is_pressed = keyInput(&window, &random, scn.getCamera());
        bool is_clicked = window.isMouseClicked();
        updateCameraAngle(&window, cmpos, lmpos, &player, scn.getCamera());
        float dmousex = cmpos[0] - lmpos[0];
        float dmousey = cmpos[1] - lmpos[1];
        lmpos[0] = cmpos[0];
        lmpos[1] = cmpos[1];

        if (is_pressed) {
            scn.applyObjectEdits(cmpos[0] - dmousex, cmpos[1] - dmousey, cmpos[0], cmpos[1]);
            if (!was_pressed) {
                scn.selectObjManipulator(cmpos[0], cmpos[1]);
            }
            was_pressed = true;
        }
        else {
            was_pressed = false;
        }

        if (is_clicked) {
            SceneObject* sel = scn.getMouseTrace(cmpos[0], cmpos[1]);
            if (sel)
                scn.setSelectedObject(sel->getId());
            else {
                scn.setSelectedObject(-1);
            }
        }

        scn.renderWithEditorFunctionality();
        window.prepareForNextFrame();
    }

    return 0;
}