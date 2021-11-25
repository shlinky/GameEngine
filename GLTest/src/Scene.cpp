#include "Scene.h"

float screenVps[12] = {
    1, -1, 0,
    1,  1, 0,
   -1,  1, 0,
   -1, -1, 0,
};

unsigned int screenIndexs[6] = {
    0, 1, 2,
    0, 3, 2
};

Scene::Scene(WindowsWindowing* win)
{
    int sizex = win->getSizeX();
    int sizey = win->getSizeY();
    renderCam = new Camera(0.0f, 3.0f, 8.0f, sizex, sizey);
    renderCam->setSensitivity(0.05);
    renderCam->setPitchLimits(-87, 87);
    renderCam->setRotation(180, 0);
    sceneBuffer = new OGLFrameBuffer();
    renderTexture = new OGLImageTexture(sizex, sizey);
    colorIDTexture = new OGLImageTexture(sizex, sizey);
    arrowTexture = new OGLImageTexture(sizex, sizey);
    outlineTexture = new OGLImageTexture(sizex, sizey);
    blank = new OGLImageTexture(sizex, sizey);

    sceneBuffer->attachColorTexture(renderTexture);

    outlineBuffer = new OGLFrameBuffer();
    outlineBuffer->attachColorTexture(blank);
    outlineBuffer->attachColorTexture(outlineTexture);

    outlineClearBuffer = new OGLFrameBuffer();
    outlineClearBuffer->attachColorTexture(outlineTexture);

    postprocess =  new OGLTexturedShader("res/shaders/pp.vert", "res/shaders/pp.frag", 1, 3);
    postprocess->addUniform<OGLUniform3FV>("selected");
    postprocess->addTexture(renderTexture);
    postprocess->addTexture(outlineTexture);

    screenquad =  new OGLVertexObject(4);
    screenquad->addAttribute(0, 3, screenVps);
    screenquad->addIndexing(screenIndexs, 6);


    OGLVertexObject* arw = new OGLVertexObject("res/models/arr.txt", true, 7);
    
    for (int i = 0; i < 3; i++) {
        arrows[i].setIsComponent(true);
        arrows[i].setMesh(arw);
        arrows[i].setScale(0.1, 0.1, 0.1);
        arrows[i].setParent(&objManipulator);
        arrows[i].createShader("res/shaders/b.vert", "res/shaders/color.frag", 1, 0);
        arrows[i].getShader()->addUniform<OGLUniform3FV>("col");
    }
    arrows[0].setRotation(0, 0, -90);
    arrows[1].setRotation(90, 0, 0);
    arrows[2].setRotation(0, 0, 0);

    float col[3] = { 1.0f, 0, 0 };
    arrows[0].getShader()->updateUniformData("col", col);
    col[0] = 0;
    col[1] = 1;
    arrows[1].getShader()->updateUniformData("col", col);
    col[1] = 0;
    col[2] = 1;
    arrows[2].getShader()->updateUniformData("col", col);
}

void Scene::addSceneObject(SceneObject* obj)
{
    sceneObjects.push_back(obj);
    obj->setId(sceneObjects.size() - 1);
}

void Scene::render()
{
    if (finalBuffer) {
        finalBuffer->bind();
        finalBuffer->clear();
    }
    for (int i = 0; i < sceneObjects.size(); i++) {
        if (sceneObjects[i]->getRenderable())
            ((RenderableSceneObject*)sceneObjects[i])->render(renderCam);
    }
}

void Scene::renderWithEditorFunctionality()
{
    //RENDERING THE SCENE INTO SCENE BUFFER
    sceneBuffer->bind();
    sceneBuffer->clear();
    for (int i = 0; i < sceneObjects.size(); i++) {
        if (sceneObjects[i]->getRenderable()) {
            if (sceneObjects[i]->getEditorMovable()) {
                float cid = ((float)i + 1) / 255.0f;
                glm::vec3 c = glm::vec3(cid, cid, cid);
                ((SceneMeshObject*)sceneObjects[i])->getShader()->updateUniformData("colorId", &(c[0]));
            }
            ((RenderableSceneObject*)sceneObjects[i])->render(renderCam);
        }
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
    sceneBuffer->unbind();

    //RENDERING THE OUTLINE
    if (selectedObject) {
        outlineBuffer->bind();
        outlineBuffer->clear();
        ((SceneMeshObject*)selectedObject)->render(renderCam);
        outlineBuffer->unbind();
    }
    else {
        outlineBuffer->bind();
        outlineBuffer->clear();
        outlineBuffer->unbind();
    }

    //FINAL RENDER
    screenquad->bind();
    postprocess->bindShaderProgram();
    if (finalBuffer) {
        finalBuffer->bind();
        finalBuffer->clear();
    }
    glDrawElements(GL_TRIANGLES, screenquad->getIndexCount(), GL_UNSIGNED_INT, (void*)0);
}

SceneObject* Scene::getMouseTrace(float x, float y)
{
    //if not editor functionality make query by doing render by itself with new shader
    unsigned char* pixels = nullptr;
    colorIDTexture->read(&pixels);
    if (((x < renderTexture->getWidth()) && (x > 0)) && ((y > 0) && (y < renderTexture->getHeight()))) {
        int sel = (int)pixels[(renderTexture->getHeight() - (int)y) * renderTexture->getWidth() * 3 + (int)x * 3];
        if (sel)
            return (sceneObjects[sel - 1]);
    }
    delete[] pixels;
    return nullptr;
}

void Scene::applyObjectEdits(float x, float y, float x1, float y1)
{
    if ((selectedObject) && (editorFunctionality)) {
        float dmousex = x1 - x;
        float dmousey = y1 - y;

        if (selectedArrow) {
            //make rotation scale and movement different functions
            glm::vec4 aDir = glm::toMat4(selectedArrow->getQuatWorldRotation()) * glm::vec4(0, 1, 0, 1);
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

void Scene::selectObjManipulator(float x, float y)
{
    unsigned char* pixels = nullptr;
    arrowTexture->read(&pixels);
    if (((x < renderTexture->getWidth()) && (x > 0)) && ((y > 0) && (y < renderTexture->getHeight()))) {
        int sel = (int)pixels[(renderTexture->getHeight() - (int)y) * renderTexture->getWidth() * 3 + (int)x * 3];
        if (sel)
            selectedArrow = &(arrows[sel - 1]);
        else {
            selectedArrow = nullptr;
        }
    }
    delete[] pixels;
}

void Scene::setSelectedObject(int id)
{
    if (id == -1) {
        selectedObject = nullptr;
    }
    else {
        selectedObject = sceneObjects[id];
        objManipulator.setIsComponent(true);
        objManipulator.setParent(selectedObject);
    }
}

Camera* Scene::getCamera()
{
    return renderCam;
}

void Scene::setCamera(Camera* cam)
{
    renderCam = cam;
}

void Scene::setEditorFunctionality(bool on)
{
    editorFunctionality = on;
    if (on) {
        sceneBuffer->attachColorTexture(colorIDTexture);
        sceneBuffer->attachColorTexture(arrowTexture);
    }
    else {
        sceneBuffer->resetColorTextures();
        sceneBuffer->attachColorTexture(renderTexture);
    }
}

void Scene::setRenderBuffer(OGLFrameBuffer* rb)
{
    finalBuffer = rb;
}

void Scene::renderHDR(bool on)
{
    for (int i = 0; i < sceneObjects.size(); i++) {
        if (sceneObjects[i]->getRenderable()) {
            ((RenderableSceneObject*)sceneObjects[i])->setHDRRendering(on);
        }
    }
}
