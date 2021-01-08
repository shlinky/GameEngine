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
//map class
//portals
//collisions
//animations and skeletal meshes bones
//blurred map

/*#define MIN -10
#define MAX 10
#define INTERVAL 0.05

float axisvp[24] = {
    10, 0, 10,
    10, 0, -10,
    -10, 0, -10,
    -10, 0, 10,
    0, 10, 10,
    0, -10, 10,
    0, -10, -10,
    0, 10, -10
};

float axisuvs[16] = {
    1.0, 0.0,
    1.0, 1.0,
    0.0, 1.0,
    0.0, 0.0,
    0.0, 1.0,
    0.0, 0.0,
    1.0, 0.0,
    1.0, 1.0
};

float axisimg[8] = {
    0,
    0,
    0,
    0,
    1,
    1,
    1,
    1
};

unsigned int axisindex[12] = {
    0, 1, 2,
    0, 3, 2,
    4, 5, 6,
    4, 7, 6
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

float execute(char s, float x, float y) {
    if (s == '+') {
        return (x + y);
    }
    if (s == '-') {
        return (x - y);
    }
    if (s == '*') {
        return (x * y);
    }
    if (s == '/') {
        return (x / y);
    }
    if (s == '^') {
        return (powf(x, y));
    }
    if (!s) {
        return (y);
    }
}

int calculateConstant(int* n, int digits) {
    int num = 0;
    for (int i = 0; i < digits; i++) {
        num += pow(10, digits - i - 1) * n[i];
    }
    return (num);
}

float evaluate_equation(string e, float x, float y) {
    float val = 0;
    char state = NULL;

    int digits = 0;
    bool number = false;
    int num = 0;
    int numl[5] = { 0 };

    float parstack[5];
    for (int i = 0; i < 5; i++) {
        parstack[i] = 0.0;
    }
    char statestack[5] = { NULL };
    int parlevel = 0;

    for (int i = 0; i < e.size(); i++) {
        if (e[i] != ' ') {
            if ((e[i] >= '0') && (e[i] <= '9')) {
                //num += (int)(e[i] - '0') * power(10, digits);
                numl[digits] = (int)(e[i] - '0');
                number = true;
                digits++;
            }
            else {
                if (number) {
                    //cout << "digits: " << digits << endl;
                    num = calculateConstant(numl, digits);
                    parstack[parlevel] = execute(statestack[parlevel], parstack[parlevel], num);
                    num = 0;
                    digits = 0;
                    number = false;
                }
                if (e[i] == 'x') {
                    parstack[parlevel] = execute(statestack[parlevel], parstack[parlevel], x);
                }
                else if (e[i] == 'y') {
                    parstack[parlevel] = execute(statestack[parlevel], parstack[parlevel], y);
                }
                else if (e[i] == '(') {
                    parlevel++;
                }
                else if (e[i] == ')') {
                    parstack[parlevel - 1] = execute(statestack[parlevel - 1],
                        parstack[parlevel - 1],
                        parstack[parlevel]);
                    parstack[parlevel] = 0.0;
                    statestack[parlevel] = NULL;
                    parlevel--;
                }
                else {
                    statestack[parlevel] = e[i];
                }
            }
        }
    }
    return (parstack[0]);
}*/

/*int main(void)
{
    int numVert = pow((MAX - MIN) / INTERVAL, 2) * 6;
    float* vp = new float[numVert * 3];
    unsigned int* index = new unsigned int[numVert];

    WindowsWindowing window(1920, 1080, "Graphing Calculator", false);

    startGLDebug();

    int sizex = window.getSizeX();
    int sizey = window.getSizeY();
    Camera cam(0.0f, 0.0f, 0.0f, &sizex, &sizey);
    cam.setPitchLimits(-87, 87);
    cam.setSensitivity(0.05);

    string equation;
    getline(cin, equation);

    int i = 0;
    int c = 0;
    const int tri2[3] = { 0, 2, 3 };

    cout << "answer: " << evaluate_equation(equation, 23, 0) << endl;

    for (float x = MIN; x <= MAX - INTERVAL; x += INTERVAL) {
        for (float y = MIN; y <= MAX - INTERVAL; y += INTERVAL) {
            float z[4] = { 0 };
            float xs[4] = { x, x + INTERVAL, x + INTERVAL, x };
            float ys[4] = { y, y, y + INTERVAL, y + INTERVAL };
            z[0] = evaluate_equation(equation, x, y);
            z[1] = evaluate_equation(equation, x + INTERVAL, y);
            z[2] = evaluate_equation(equation, x + INTERVAL, y + INTERVAL);
            z[3] = evaluate_equation(equation, x, y + INTERVAL);
            bool onegood = false;

            for (int n = 0; n < 4; n++) {
                if (z[n] < MIN) {
                    z[n] = MIN;
                }
                else if (z[n] > MAX) {
                    z[n] = MAX;
                }
                else {
                    onegood = true;
                }
            }

            if (onegood) {
                for (int n = 0; n < 3; n++) {
                    vp[c] = xs[n];
                    vp[c + 1] = z[n];
                    vp[c + 2] = ys[n];

                    index[i] = i;

                    c += 3;
                    i += 1;
                }
                for (int n = 0; n < 3; n++) {
                    vp[c] = xs[tri2[n]];
                    vp[c + 1] = z[tri2[n]];
                    vp[c + 2] = ys[tri2[n]];

                    index[i] = i;

                    c += 3;
                    i += 1;
                }
            }
        }
    }

    OGLVertexObject curve = OGLVertexObject(numVert);
    curve.addAttribute(0, 3, vp);
    curve.addIndexing(index, numVert);

    OGLVertexObject axis = OGLVertexObject(8);
    axis.addAttribute(0, 3, axisvp);
    axis.addAttribute(1, 2, axisuvs);
    axis.addAttribute(2, 1, axisimg);
    axis.addIndexing(axisindex, 12);

    OGLShaderProgram s = OGLShaderProgram("res/shaders/graphing_calc.vert", "res/shaders/graphing_calc.frag", 2);
    s.addUniform<OGLUniformMat4FV>("coolbeans");
    s.addUniform<OGLUniformMat4FV>("world");

    OGLTexturedShader as = OGLTexturedShader("res/shaders/axis.vert", "res/shaders/axis.frag", 1, 2);
    as.addUniform<OGLUniformMat4FV>("coolbeans");
    as.addTexture("res/shaders/bob.jpg");
    as.addTexture("res/models/kcolor.png");

    double lmpos[2] = {};
    double cmpos[2] = {};

    window.getMousePos(lmpos);

    window.prepareForNextFrame();

    while (!window.isWindowClosing())
    {
        window.getMousePos(cmpos);
        updateCameraAngle(cmpos, lmpos, &cam);
        keyInput(&window, &cam);

        lmpos[0] = cmpos[0];
        lmpos[1] = cmpos[1];

        float campos[3];
        cam.getPosition(campos);

        glm::mat4 v = glm::make_mat4(cam.getTransMat());
        glm::mat4 mvp = v;
        glm::mat4 i = glm::identity<glm::mat4>();
        s.updateUniformData("coolbeans", &mvp);
        as.updateUniformData("coolbeans", &mvp);
        s.updateUniformData("world", &i);

        //make a render function in env cube class
        //bind and render function in scene objects; render calls bind

        curve.bind();
        s.bindShaderProgram();
        glDrawElements(GL_TRIANGLES, curve.getVertexCount(), GL_UNSIGNED_INT, (void*)0);

        axis.bind();
        as.bindShaderProgram();
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (void*)0);

        window.prepareForNextFrame();
    }


    return 0;
}*/