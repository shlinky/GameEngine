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
#include "HDRCubeMap.h"
using namespace std;

//add scenemesh class
//add scene class
//HASH connecting names to objects
//multiple cameras if only one select that one
//lights are also scene objects with enum for which type it is
//tick for scene and scene objects
//each scene mesh has a pointer to the scene
//ray cast is in the scene class so it has access to all scene objects
//ray cast gives back pointer to object it hit

//all scene objects have properties that is an array of various strings so u can search scene for all objects with a certain property
//u can get a scene object back by its name

//map class
//portals
// use normals for orientation

//collisions
//animations and skeletal meshes bones
//blurred map
//lihgts as scene objects
//collisions
//physics
//physx implementation
//steamworks
//audio

//ray casting (path tracing)
//ui through imgui

//all sceneobjects have a tick function
//all sceneobjects have a component array (childComponents) that can be filled with other sceneobjects (COMPONENT MATH) (relative positions)
//sceneobject has a function create component where it is added to the scene and to the component array
//and have renderable property
//and have pointer to scene
//rendering pipeline like shadows and such is done in scene calss
//make renderablesceneobject class which mesh derives from and particle and other such things
//renderable scene object has hidden property and and render function and lit property and a shader
//possibly every renderable scene object gets a setRenderFrameBuffer function
//SCENE OBECTS CAN HAVE CAMERAS AND other objects in them (components)
//but also add it to the scene during construction (for lights and others) (all components)
//physics bodies are not renderable but scene objects

//whenever something needs access to something give it a pointer to it

//scene has a main fb for post processing and ppl can access the textures and the buffer
//scene has a render function with cam param and not
//a variable for whethered its deffered rendering or not

//displacement mapping
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


void keyInput(WindowsWindowing* w, SceneMeshObject* g, Camera* c) {
    glm::vec3 p = g->getPosition();
    glm::vec3 oc = c->getPosition();
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
        cout << "pressed" << endl;
    }
    if (w->isMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT)) {
        cout << "dpressed" << endl;
    }
    //glm::vec3 d = c->getPosition() - oc;
    //glm::vec3 npos = (p + d);
    //cout << npos.x << ' ' << npos.y << ' ' << npos.z << endl;
   // g->setPosition(npos.x, npos.y, npos.z);
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

/*float screen[16] = {
    1.0, 0.0,
    1.0, 1.0,
    0.0, 1.0,
    0.0, 0.0,
    0.0, 1.0,
    0.0, 0.0,
    1.0, 0.0,
    1.0, 1.0
};*/

int main(void)
{
    WindowsWindowing window(1920, 1080, "Application", false);

    startGLDebug();

    int sizex = window.getSizeX();
    int sizey = window.getSizeY();
    Camera cam(0.0f, 0.0f, 8.0f, sizex, sizey);
    cam.setSensitivity(0.05);
    cam.setPitchLimits(-87, 87);

    Camera pcam(0.0f, 0.0f, 10.0f, sizex, sizey);
    cam.setSensitivity(0.05);
    cam.setPitchLimits(-87, 87);

    OGLFrameBuffer fb;
    OGLImageTexture col(sizex, sizey);
    OGLImageTexture depth(sizex, sizey, GL_DEPTH_COMPONENT);
    fb.attachColorTexture(&col);
    fb.attachDepthTexture(&depth);

    OGLFrameBuffer fb1;
    OGLImageTexture col1(sizex, sizey);
    fb1.attachColorTexture(&col1);

    OGLTexturedShader postprocess = OGLTexturedShader("res/shaders/pp.vert", "res/shaders/pp.frag", 0, 2);
    postprocess.addTexture(&col);
    postprocess.addTexture(&depth);

    OGLVertexObject screenquad = OGLVertexObject(4);
    screenquad.addAttribute(0, 3, screenvp);
    screenquad.addIndexing(screenindex, 6);

    OGLVertexObject portal = OGLVertexObject(4);
    portal.addAttribute(0, 3, screenvp);
    portal.addIndexing(screenindex, 6);

    SceneMeshObject portala = SceneMeshObject(5, 5, -10);
    portala.setMesh(&portal);
    portala.createShader("res/shaders/p.vert", "res/shaders/p.frag");
    portala.getShader()->addTexture(&col1);
    portala.getShader()->addTexture("res/shaders/frame.png");

    SceneMeshObject portalb = SceneMeshObject(0, 0, 10);
    portalb.setMesh(&portal);
    portalb.setShader(portala.getShader());
    portalb.setRotation(0, 0, 0);
    OGLVertexObject model1("res/models/gucci.txt", true);

    SceneObject player = SceneObject();

    SceneMeshObject random = SceneMeshObject(0.45, -0.2, -2.3);
    random.setRotation(180, 0, 0);
    random.setScale(0.2, 0.2, 0.2);
    random.setIsComponent(true);
    random.setParent(&player);
    random.setMesh("res/models/test.txt");
    random.createShader("res/shaders/b.vert", "res/shaders/pgun.frag");
    random.getShader()->addTexture("res/models/v_portalgun.png");
    random.getShader()->addTexture("res/models/v_portalgun_s.jpg");

    SceneMeshObject speak = SceneMeshObject(0.45, -0.2, -2.3);
    speak.setRotation(180, 0, 0);
    speak.setScale(0.1, 0.1, 0.1);
    speak.setMesh("res/models/speak.txt");
    speak.createShader("res/shaders/b.vert", "res/shaders/pbr_simp.frag");

    //speak.getShader()->addTexture("res/models/PreviewSphere" + to_string(i) + "_Sphere_BaseColor.png");
    //speak.getShader()->addTexture("res/models/PreviewSphere" + to_string(i) + "_Sphere_Normal.png");
    //speak.getShader()->addTexture("res/models/PreviewSphere" + to_string(i) + "_Sphere_OcclusionRoughnessMetallic.png");

    SceneMeshObject* sp[5] = { 0 };
    for (int i = 0; i < 5; i++) {
        sp[i] = new SceneMeshObject(&model1);
        sp[i]->setPosition(1 + (2 * i), 1, 1);
        sp[i]->createShader("res/shaders/b.vert", "res/shaders/pbr.frag");
        sp[i]->getShader()->addTexture("res/models/PreviewSphere" + to_string(i) + "_Sphere_BaseColor.png");
        sp[i]->getShader()->addTexture("res/models/PreviewSphere" + to_string(i) + "_Sphere_Normal.png");
        sp[i]->getShader()->addTexture("res/models/PreviewSphere" + to_string(i) + "_Sphere_OcclusionRoughnessMetallic.png");
    }

    HDRCubeMap hdr = HDRCubeMap("res/shaders/bob.hdr", 512);
    hdr.save("wierdgear");

    double lmpos[2] = {};
    double cmpos[2] = {};

    window.getMousePos(lmpos);
    
    window.prepareForNextFrame();
    /* Loop until the user closes the window */
    while (!window.isWindowClosing())
    {
        window.getMousePos(cmpos);
        keyInput(&window, &random, &cam);
        updateCameraAngle(cmpos, lmpos, &player, &cam);
        /*if (cam.getPosition().z <= -10) {
            cam.setPosition(cam.getPosition().x - 5, cam.getPosition().y - 5, 10 + cam.getPosition().z + 10);
        }

        if (cam.getPosition().z >= 10) {
            cam.setPosition(5 + cam.getPosition().x, 5 + cam.getPosition().y, -10 + (cam.getPosition().z - 10));
        }*/
        //cout << cam.getPosition().x << ' ' << cam.getPosition().y << ' ' << cam.getPosition().z << endl;
        
        lmpos[0] = cmpos[0];
        lmpos[1] = cmpos[1];

        //make camera go back behind the portal
        //uvs with the world transforms

        /*glm::vec3 diff = (cam.getPosition() - glm::vec3(5, 5, -10));
        pcam.setPosition(diff.x, diff.y, diff.z + 10);
        pcam.setRotation(cam.getYaw(), cam.getPitch());

        fb1.bind();
        fb1.clear();
        for (int i = 0; i < 4; i++) {
            sp[i]->render(&pcam);
        }
        random.render(&pcam);
        fb.bind();
        fb.clear();
        portala.render(&cam);

        diff = (cam.getPosition() - glm::vec3(0, 0, 10));
        pcam.setPosition(diff.x + 5, diff.y + 5, diff.z - 10);
        pcam.setRotation(cam.getYaw(), cam.getPitch());

        fb1.bind();
        fb1.clear();
        for (int i = 0; i < 4; i++) {
            sp[i]->render(&pcam);
        }
        random.render(&pcam);
        fb.bind();
        portalb.render(&cam);*/
        for (int i = 0; i < 5; i++) {
            sp[i]->render(&cam);
        }
        //cout << random.getPosition().x << ' ' << random.getPosition().y << ' ' << random.getPosition().z << endl;
        random.render(&cam);
        //speak.render(&cam);
        //fb.unbind();
        //screenquad.bind();
        //postprocess.bindShaderProgram();
        //glDrawElements(GL_TRIANGLES, screenquad.getIndexCount(), GL_UNSIGNED_INT, (void*)0);

        window.prepareForNextFrame();
    }

    return 0;
}


//v = glm::make_mat4(cam.getTransMat(false));
        //glm::mat4 mvpc = v * glm::scale(glm::identity<glm::mat4>(), glm::vec3(100, 100, 100));
        //shaderpc.updateUniformData("coolbeans", &mvpc);

        //make a render function in env cube class
        //env cube is a renderable scene object
        //bind and render function in scene objects; render calls bind
        /**glDepthMask(GL_FALSE);
        envcube.bind();
        shaderpc.bindShaderProgram();
        glDrawElements(GL_TRIANGLES, envcube.getVertexCount(), GL_UNSIGNED_INT, (void*)0);
        glDepthMask(GL_TRUE);**/


        //OGLTexturedShader shaderp("res/shaders/b.vert", "res/shaders/dir1.frag", 5, 4);
            //shaderp.addUniform<OGLUniformMat4FV>("coolbeans");
            //shaderp.addUniform<OGLUniformMat4FV>("world");
            //shaderp.addUniform<OGLUniform3FV>("light_position", &lightPosition);
            //shaderp.addUniform<OGLUniform3FV>("light_color", &lightColor);
            //shaderp.addUniform<OGLUniform3FV>("camera_position");
            //shaderp.addCubemapTexture(cubeMapNames);
            //shaderp.addTexture("res/models/kcolor.png");
            //shaderp.addTexture("res/models/knorm.png");
            //shaderp.addTexture("res/models/korm.png");

//OGLTexturedShader shaderpc("res/shaders/bc.vert", "res/shaders/wcube.frag", 4, 1);
    //shaderpc.addUniform<OGLUniformMat4FV>("coolbeans");
    //shaderpc.addUniform<OGLUniform3FV>("light_position", &lightPosition);
    //shaderpc.addUniform<OGLUniform3FV>("light_color", &lightColor);
    //shaderpc.addUniform<OGLUniform3FV>("camera_position");
    //shaderpc.addCubemapTexture(cubeMapNames);