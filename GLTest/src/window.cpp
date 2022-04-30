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
#include "PortalObject.h"
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

        c->rotatePitch(dY);
        c->rotateYaw(dX);

        float yaw = c->getYaw();
        float ptch = c->getPitch();
        //cout << yaw << endl;
        g->setQuatRotation(c->getQuatRotation());
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

glm::vec3 line_plane(glm::vec3 b, glm::vec3 m, glm::vec3 n, glm::vec3 p) {
    float d = glm::dot((p - b), n) / glm::dot(m, n);
    return (b + (m * d));
}

int main(void)
{
    WindowsWindowing window(1920, 1080, "Application", false);

    startGLDebug();

    OGLVertexObject model1("res/models/gucci.txt", true);

    SceneObject player = SceneObject();

    cout << "started1" << endl;

    OGLImageTexture brdff = OGLImageTexture("res/shaders/brdf.png");
    OGLCubeMapTexture cm = OGLCubeMapTexture(512);
    OGLCubeMapTexture c("res/shaders/bob1.hdr", 512);
    OGLCubeMapTexture* irr = c.createIrradianceMap(256);
    OGLCubeMapTexture* spec = c.createPrefilteredSpec(256);
    EnvCube env = EnvCube(&cm);

    /*SceneMeshObject* sp[8] = { 0 };
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
    }*/

    SceneMeshObject ceiling = SceneMeshObject("res/models/floor.txt");
    ceiling.createShader("res/shaders/b.vert", "res/shaders/pbr_mixed_tile.frag", 6, 8);
    ceiling.setScale(80, 80, 80);
    ceiling.setPosition(0, 20, 0);
    ceiling.setRotation(180, 0, 0);
    ceiling.getShader()->addTexture("res/models/plane_divided_DefaultMaterial_BaseColor.png");
    ceiling.getShader()->addTexture("res/models/plane_divided_DefaultMaterial_Normal.png");
    ceiling.getShader()->addTexture("res/models/plane_divided_DefaultMaterial_OcclusionRoughnessMetallic.png");
    ceiling.getShader()->addTexture(irr);
    ceiling.getShader()->addTexture(&brdff);
    ceiling.getShader()->addTexture(spec);
    ceiling.getShader()->addUniform<OGLUniform3FV>("colorId");

    SceneMeshObject floor = SceneMeshObject("res/models/floor.txt");
    floor.createShader("res/shaders/b.vert", "res/shaders/pbr_mixed_tile.frag", 6, 8);
    floor.setScale(120, 120, 120);
    floor.setPosition(0, 0, 0);
    floor.setRotation(0, 0, 0);
    floor.getShader()->addTexture("res/models/carpet_BaseColor.png");
    floor.getShader()->addTexture("res/models/carpet_Normal.png");
    floor.getShader()->addTexture("res/models/carpet_OcclusionRoughnessMetallic.png");
    floor.getShader()->addTexture(irr);
    floor.getShader()->addTexture(&brdff);
    floor.getShader()->addTexture(spec);
    floor.getShader()->addUniform<OGLUniform3FV>("colorId");

    Scene scn = Scene(&window);
    //scn.addSceneObject(&env);
    OGLCubeMapTexture scnCapture(256, true);
    EnvCube e(&c);
    scn.addSceneObject(&e);
    scn.addSceneObject(&floor);
    scn.addSceneObject(&ceiling);

    SceneMeshObject walls[4];
    for (int i = 0; i < 4; i++) {
        walls[i].setMesh("res/models/floor.txt");
        walls[i].createShader("res/shaders/b.vert", "res/shaders/pbr_mixed_tile.frag", 6, 8);
        walls[i].setScale(80, 80, 80);
        walls[i].getShader()->addTexture("res/models/plane_divided_DefaultMaterial_BaseColor.png");
        walls[i].getShader()->addTexture("res/models/plane_divided_DefaultMaterial_Normal.png");
        walls[i].getShader()->addTexture("res/models/plane_divided_DefaultMaterial_OcclusionRoughnessMetallic.png");
        walls[i].getShader()->addTexture(irr);
        walls[i].getShader()->addTexture(&brdff);
        walls[i].getShader()->addTexture(spec);
        walls[i].getShader()->addUniform<OGLUniform3FV>("colorId");
        scn.addSceneObject(&walls[i]);
    }

    walls[0].setRotation(90, 0, 0);
    walls[0].setPosition(0, 0, -20);
    walls[1].setRotation(-90, 0, 0);
    walls[1].setPosition(0, 0, 20);
    walls[2].setRotation(0, 0, 90);
    walls[2].setPosition(20, 0, 0);
    walls[3].setRotation(0, 0, -90);
    walls[3].setPosition(-20, 0, 0);

    /*SceneMeshObject knf = SceneMeshObject("res/models/knife.txt");
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
    knf.getShader()->addUniform<OGLUniform3FV>("colorId");*/

    SceneMeshObject random = SceneMeshObject(0.45, -0.3, -2.3);
    random.setRotation(0, 180, 0);
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

    SceneMeshObject knf = SceneMeshObject("res/models/chair.txt");
    knf.createShader("res/shaders/b.vert", "res/shaders/pbr_mixed.frag", 6, 8);
    knf.setPosition(3.2, 1, 0);
    knf.setScale(0.2, 0.2, 0.2);
    knf.getShader()->addTexture("res/models/chair_BaseColor.png");
    knf.getShader()->addTexture("res/models/chair_Normal.png");
    knf.getShader()->addTexture("res/models/chair_OcclusionRoughnessMetallic.png");
    knf.getShader()->addTexture(irr);
    knf.getShader()->addTexture(&brdff);
    knf.getShader()->addTexture(spec);
    knf.getShader()->addUniform<OGLUniform3FV>("colorId");
    
    SceneMeshObject book = SceneMeshObject("res/models/book.txt");
    book.setPosition(3.2, 1.5, -7);
    book.setScale(0.1, 0.1, 0.1);
    book.createShader("res/shaders/b.vert", "res/shaders/pbr_mixed.frag", 6, 8);
    book.getShader()->addTexture("res/models/book1_DefaultMaterial_BaseColor.png");
    book.getShader()->addTexture("res/models/book1_DefaultMaterial_Normal.png");
    book.getShader()->addTexture("res/models/book1_DefaultMaterial_OcclusionRoughnessMetallic.png");
    book.getShader()->addTexture(irr);
    book.getShader()->addTexture(&brdff);
    book.getShader()->addTexture(spec);
    book.getShader()->addUniform<OGLUniform3FV>("colorId");

    //irr = c.createIrradianceMap(256);
    //spec = c.createPrefilteredSpec(256);

    SceneMeshObject spk = SceneMeshObject("res/models/spk.txt");
    spk.setPosition(-3.2, 1.5, -7);
    spk.setScale(1, 1, 1);
    spk.setRotation(0, 180, 0);
    spk.createShader("res/shaders/b.vert", "res/shaders/pbr_mixed.frag", 6, 8);
    spk.getShader()->addTexture("res/models/speakers_low_DefaultMaterial_BaseColor.png");
    spk.getShader()->addTexture("res/models/speakers_low_DefaultMaterial_Normal.png");
    spk.getShader()->addTexture("res/models/speakers_low_DefaultMaterial_OcclusionRoughnessMetallic.png");
    spk.getShader()->addTexture(irr);
    spk.getShader()->addTexture(&brdff);
    spk.getShader()->addTexture(spec);
    spk.getShader()->addUniform<OGLUniform3FV>("colorId");

    //SceneMeshObject spk1 = spk;
    //spk1.setPosition(-4.2, 1.5, -7);
    scn.addSceneObject(&random);
    scn.addSceneObject(&knf);
    scn.addSceneObject(&book);
    scn.addSceneObject(&spk);
    //scn.addSceneObject(&spk1);
    /*sp[5] = &knf;
    sp[6] = &floor;
    sp[7] = &random;*/





    //OGLImageTexture necol(colbb.getWidth(), colbb.getHeight(), pixels);
    double lmpos[2] = {};
    double cmpos[2] = {};

    window.getMousePos(lmpos);

    window.prepareForNextFrame();

    bool was_pressed = false;

    //sp[5]->setHDRRendering(true);
    //for (int i = 0; i < 8; i++) {
    //    scn.addSceneObject(sp[i]);
    //}
    OGLCubeMapTexture* irrscn[6]; 
    OGLCubeMapTexture* specscn[6];
    scn.renderHDR(true);
    for (int i = 1; i < scn.getObjectCount(); i++) {
        SceneMeshObject* m = (SceneMeshObject*)scn.getObject(i);
        glm::vec3 pos = m->getPosition();
        scn.getCamera()->setPosition(pos.x, pos.y, pos.z);
        m->setHidden(true);
        scnCapture.renderIntoCubemap(&scn);
        irrscn[i] = scnCapture.createIrradianceMap(100);
        specscn[i] = scnCapture.createPrefilteredSpec(100);
        m->getShader()->changeTexture(irrscn[i], 3);
        m->getShader()->changeTexture(specscn[i], 5);
        m->setHidden(false);
        m->getShader()->addUniform<OGLUniformFloat>("isPortalCapture");
        m->getShader()->addUniform<OGLUniform3FV>("portalLoc");
        m->getShader()->addUniform<OGLUniform3FV>("portalNorm");
    }
    scn.renderHDR(false);
    //scn.getObject(0);
    /*SceneMeshObject* first = ((SceneMeshObject*)scn.getObject(6));
    first->getShader()->addUniform<OGLUniformFloat>("TwoD");
    float td = 1.0f;
    first->getShader()->updateUniformData("TwoD", &td);

    OGLFrameBuffer tdBuffer;
    OGLImageTexture tdT(1000, 1000);
    tdBuffer.attachColorTexture(&tdT);
    tdBuffer.bind();
    tdBuffer.clear();
    first->render(scn.getCamera());
    tdBuffer.unbind();

    tdT.save("uvrender.jpg");

    td = 0.0f;
    first->getShader()->updateUniformData("TwoD", &td);*/

    scn.setEditorFunctionality(true);
    env.setCubeMap(&cm);

    int refcount = 0;
    // Loop until the user closes the window 

    //OGLCubeMapTexture scnCapture(100, true);
    scn.renderHDR(false);

    PortalObject p1(window.getSizeX(), window.getSizeY());
    PortalObject p2(window.getSizeX(), window.getSizeY());
    p1.setSecondPortal(&p2);
    p2.setSecondPortal(&p1);
    p1.setPosition(10, 10, -10);
    p2.setPosition(-10, 10, -10);
    p1.setScale(3, 3, 3);
    p2.setScale(3, 3, 3);
    p1.setScene(&scn);
    p2.setScene(&scn);
    p1.setRotation(30, 0, 0);
    //p2.setRotation(-90, 0, 0);

    scn.addSceneObject(&p1);
    scn.addSceneObject(&p2);




    //create a separate file that does the engine stuff only
    //create an object using the window and scene
    //and call render (which also take care of rendering the scene)
    //add tick
    //initialization list for object
    while (!window.isWindowClosing())
    {
        //cout << "started" << endl;
        window.getMousePos(cmpos);
        bool is_pressed = keyInput(&window, &random, scn.getCamera());
        bool is_right_clicked = false;
        if (window.isMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT)) {
            is_right_clicked = true;
        }
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
            SceneObject* sel = scn.getMouseTrace(window.getSizeX() / 2, window.getSizeY() / 2);
            if (sel) {

                if (sel->getId() == 1) {
                    glm::vec3 npl = line_plane(scn.getCamera()->getPosition(), scn.getCamera()->getForwardDir(), glm::vec3(0, 1, 0), glm::vec3(0, 0, 0));
                    //p1.setPosition(npl.x, npl.y + 0.05, npl.z);
                    //it should take only object and camera
                    p1.movePortal(npl, glm::angleAxis(glm::radians(-90.0f), glm::vec3(1, 0, 0)));
                }
                else {
                    scn.setSelectedObject(sel->getId());
                }
            }
            else {
                scn.setSelectedObject(-1);
            }
        }
        p1.captureView();
        p2.captureView();

        /*scn.renderHDR(true);
        glm::vec3 opos = scn.getCamera()->getPosition();
        glm::vec3 pos;
        if (refcount == 5) {
            pos = player.getPosition();
        }
        else {
            pos = sp[refcount]->getPosition();
        }
        scn.getCamera()->setPosition(pos.x, pos.y, pos.z);
        sp[refcount]->setHidden(true);
        scnCapture.renderIntoCubemap(&scn);
        scnCapture.createIrradianceMap(50, irrscn[refcount]);
        scnCapture.createPrefilteredSpec(50, specscn[refcount]);
        sp[refcount]->setHidden(false);
        scn.renderHDR(false);
        scn.getCamera()->setPosition(opos.x, opos.y, opos.z);
        refcount++;
        refcount = refcount % 6;*/

        scn.renderWithEditorFunctionality();
        window.prepareForNextFrame();

        //cahgne to not getting objects from swp but from scene using ids and object movable.
    }

    return 0;
}