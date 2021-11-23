#include "Scene.h"

Scene::Scene(WindowsWindowing* win)
{
    int sizex = win->getSizeX();
    int sizey = win->getSizeY();
    renderCam = new Camera(0.0f, 3.0f, 8.0f, sizex, sizey);
    renderCam->setSensitivity(0.05);
    renderCam->setPitchLimits(-87, 87);
    renderCam->setRotation(180, 0);
    mainBuffer = new OGLFrameBuffer();
    renderTexture = OGLImageTexture(sizex, sizey);
    mainBuffer->attachColorTexture(&renderTexture);
    
    objManipulator.setIsComponent(true);
}

void Scene::addSceneObject(SceneObject* obj)
{
    sceneObjects.push_back(obj);
}

void Scene::render()
{
    mainBuffer->bind();
    mainBuffer->clear();
    for (int i = 0; i < sceneObjects.size(); i++) {
        //if object renderable (make this a virtual base class)
        ((SceneMeshObject*)sceneObjects[i])->render(renderCam);
    }
}

void Scene::renderWithEditorFunctionality()
{
    mainBuffer->bind();
    mainBuffer->clear();
    for (int i = 0; i < 8; i++) {
        float cid = ((float)i + 1) / 255.0f;
        glm::vec3 c = glm::vec3(cid, cid, cid);
        ((SceneMeshObject*)sceneObjects[i])->getShader()->updateUniformData("colorId", &(c[0]));
        ((SceneMeshObject*)sceneObjects[i])->render(renderCam);
    }

    glClear(GL_DEPTH_BUFFER_BIT);
    //will be done in separate arrow class maybe
    float arrowScale = 0.07;
    //make based on world position
    float sscale = arrowScale * glm::distance(renderCam->getPosition(), objManipulator.getPosition());
    objManipulator.setScale(sscale, sscale, sscale);
    for (int i = 0; i < 3; i++) {
        arrows[i].render(renderCam);
    }
    mainBuffer->unbind();

    if (selectedObject) {
        outlineBuffer->bind();
        outlineBuffer->clear();
        ((SceneMeshObject*)selectedObject)->render(renderCam);
        outlineBuffer->unbind();

        // do in function for setting selected object
        //glm::vec3 spos = selectedObject->getPosition();
        //objManipulator.setPosition(spos.x, spos.y, spos.z);
    }
    else {
        outlineBuffer->bind();
        outlineBuffer->clear();
        outlineBuffer->unbind();
    }

    screenquad->bind();
    postprocess->bindShaderProgram();
    glDrawElements(GL_TRIANGLES, screenquad->getIndexCount(), GL_UNSIGNED_INT, (void*)0);
}

SceneObject* Scene::getMouseTrace(float x, float y)
{
    //if not editor functionality make query by doing render by itself with new shader
    unsigned char* pixels = nullptr;
    colorIDTexture.read(&pixels);
    if (((x < renderTexture.getWidth()) && (x > 0)) && ((y > 0) && (y < renderTexture.getHeight()))) {
        int sel = (int)pixels[(renderTexture.getHeight() - (int)y) * renderTexture.getWidth() * 3 + (int)x * 3];
        return (sceneObjects[sel]);
    }
    return nullptr;
}

void Scene::applyObjectEdits(float x, float y, float x1, float y1)
{
    if ((selectedObject) && (editorFunctionality)) {
        float dmousex = x1 - x;
        float dmousey = y1 - y;
        unsigned char* pixels = nullptr;
        int arrow = 0;
        arrowTexture.read(&pixels);
        if (((x < renderTexture.getWidth()) && (x1 > 0)) && ((y1 > 0) && (y1 < renderTexture.getHeight()))) {
            int sel = (int)pixels[(renderTexture.getHeight() - (int)y1) * renderTexture.getWidth() * 3 + (int)x1 * 3];
            arrow = sel;
        }

        if (arrow) {
            //make rotation scale and movement different functions
            glm::vec4 aDir = glm::toMat4(arrows[arrow - 1].getQuatWorldRotation()) * glm::vec4(0, 1, 0, 1);
            glm::vec3 adir = aDir;
            glm::vec2 sDir = glm::vec2(glm::dot(renderCam->getRightDir(), adir), glm::dot(glm::normalize(glm::cross(renderCam->getRightDir(), renderCam->getForwardDir())), adir));
            glm::vec2 mouseDir = glm::vec2(dmousex, -1 * dmousey);
            float change = glm::dot(mouseDir, sDir);
            adir = adir * change * 0.01f * glm::distance(renderCam->getPosition(), objManipulator.getPosition());
            glm::vec3 spos = selectedObject->getPosition() + adir;

            selectedObject->setPosition(spos.x, spos.y, spos.z);
        }
    }
}

void Scene::setSelectedObject(int id)
{
    selectedObject = sceneObjects[id];
    objManipulator.setParent(selectedObject);
}

Camera* Scene::getCamera()
{
    return renderCam;
}

void Scene::setEditorFunctionality(bool on)
{
    editorFunctionality = on;
    if (on) {
        mainBuffer->attachColorTexture(&colorIDTexture);
        mainBuffer->attachColorTexture(&arrowTexture);
    }
}
