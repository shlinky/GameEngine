#include "PortalObject.h"

PortalObject::PortalObject(int sizex, int sizey)
{
    OGLVertexObject* mesh = new OGLVertexObject(4);
	mesh->addAttribute(0, 3, screenvp);
	mesh->addIndexing(screenindex, 6);
    setMesh(mesh);

    fb = new OGLFrameBuffer();
    col = new OGLImageTexture(sizex, sizey, true);
    fb->attachColorTexture(col);

    createShader("res/shaders/p.vert", "res/shaders/p.frag");
    getShader()->addTexture(col);
    getShader()->addTexture("res/shaders/frame.png");

    pcam = new Camera(0, 0, 0, sizex, sizey);
    setRotation(0, 0, 0);
}

void PortalObject::render(Camera* cam)
{
    this->SceneMeshObject::render(cam);
}

void PortalObject::setSecondPortal(PortalObject* portalb)
{
    this->portalb = portalb;
}

//might be in all renderable objects
void PortalObject::setScene(Scene* s)
{
    scn = s;
}

void PortalObject::captureView()
{
    //portal camera positon transformations
    Camera* cam = scn->getCamera();
    glm::vec3 diff = (cam->getPosition() - getPosition());
    diff = diff * glm::toMat3(glm::normalize(getQuatWorldRotation()));
    diff.x *= -1;
    diff.z *= -1;
    diff = glm::toMat3(portalb->getQuatWorldRotation()) * diff;

    glm::vec3 ppos = diff + portalb->getPosition();
    pcam->setPosition(ppos.x, ppos.y, ppos.z);


    //portal camera rotation transformations (Make the camera all quaternion based; no yaw pitch nonsense)
    glm::quat oQuat = glm::angleAxis(glm::radians(cam->getPitch()), glm::vec3(1.0, 0.0, 0.0));
    oQuat = glm::angleAxis(glm::radians(-90-cam->getYaw()), glm::vec3(0.0, 1.0, 0.0)) * oQuat;

    oQuat = glm::angleAxis(glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0)) * (glm::normalize(glm::inverse(getQuatWorldRotation())) * oQuat);
    oQuat = portalb->getQuatWorldRotation() * oQuat;

    pcam->setQuatRotation(oQuat);


    //rendering into portal texture
    float p = 1.0f;
    glm::vec3 sNormal = glm::toMat3(portalb->getQuatRotation()) * glm::vec3(0, 0, 1);
    glm::vec3 l = portalb->getPosition();
    for (int i = 0; i < scn->getObjectCount(); i++) {
        SceneObject* o = scn->getObject(i);
        if (o->getEditorMovable()) {
            SceneMeshObject* m = (SceneMeshObject*)o;
            m->getShader()->updateUniformData("isPortalCapture", &p);
            m->getShader()->updateUniformData("portalLoc", &(l[0]));
            m->getShader()->updateUniformData("portalNorm", &(sNormal[0]));
        }
    }

    this->setHidden(true);
    portalb->setHidden(true);
    OGLFrameBuffer* ob = scn->getFinalBuffer();

    scn->setRenderBuffer(fb);
    scn->renderHDR(true);
    scn->setCamera(pcam);
    scn->render();

    scn->setCamera(cam);
    scn->renderHDR(false);
    scn->setRenderBuffer(ob);
    this->setHidden(false);
    portalb->setHidden(false);

    p = 0.0f;
    for (int i = 0; i < scn->getObjectCount(); i++) {
        SceneObject* o = scn->getObject(i);
        if (o->getEditorMovable()) {
            SceneMeshObject* m = (SceneMeshObject*)o;
            m->getShader()->updateUniformData("isPortalCapture", &p);
        }
    }
}

void PortalObject::movePortal(glm::vec3 p, glm::quat r)
{
    glm::vec3 up = glm::toMat3(r) * glm::vec3(0, 1, 0);
    glm::vec3 right = glm::toMat3(r) * glm::vec3(1, 0, 0);

    glm::vec3 camUp = glm::cross(scn->getCamera()->getRightDir(), scn->getCamera()->getForwardDir());
    glm::vec2 portalSurfaceRotation = glm::normalize(glm::vec2(glm::dot(camUp, right), glm::dot(camUp, up)));

    float sTheta = atan2(portalSurfaceRotation.y, portalSurfaceRotation.x);

    glm::vec3 sNormal = glm::toMat3(r) * glm::vec3(0, 0, 1);
    glm::quat sRot = glm::angleAxis(sTheta - glm::radians(90.0f), sNormal);
    glm::quat portalFinalRot = sRot * r;

    glm::vec3 finalP = p + (0.05f * sNormal);
    setPosition(finalP.x, finalP.y, finalP.z);
    rotation = portalFinalRot;
}

