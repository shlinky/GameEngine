#include "PortalObject.h"
#include "..\include\PortalObject.h"

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
    Camera* cam = scn->getCamera();
    glm::vec3 diff = (cam->getPosition() - getPosition());
    if (glm::length(getQuatWorldRotation()) < 0.01) {
        diff = (diff * glm::toMat3(getQuatWorldRotation()));
    }
    else {
        diff = (diff * glm::toMat3(glm::inverse(getQuatWorldRotation())));
    }
    diff.x *= -1;
    diff.z *= -1;
    diff = diff * glm::toMat3(portalb->getQuatWorldRotation());

    glm::vec3 ppos = diff + portalb->getPosition();
    pcam->setPosition(ppos.x, ppos.y, ppos.z);

    float newYaw = (cam->getYaw() - getRotation().x) + 180 + portalb->getRotation().x;
    float newPitch = (getRotation().y + cam->getPitch()) - portalb->getRotation().y;

    pcam->setRotation(newYaw, newPitch);

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
}
