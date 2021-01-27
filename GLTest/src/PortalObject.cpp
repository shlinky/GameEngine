#include "PortalObject.h"
#include "..\include\PortalObject.h"

PortalObject::PortalObject(float x, float y, float z, int sizex, int sizey)
{
	setPosition(x, y, z);
    OGLVertexObject* mesh = new OGLVertexObject(4);
	mesh->addAttribute(0, 3, screenvp);
	mesh->addIndexing(screenindex, 6);
    setMesh(mesh);

    fb = new OGLFrameBuffer();
    OGLImageTexture col(sizex, sizey);
    fb->attachColorTexture(&col);

    createShader("res/shaders/p.vert", "res/shaders/p.frag");
    getShader()->addTexture(&col);
    getShader()->addTexture("res/shaders/frame.png");

    pcam = new Camera(0, 0, 0, sizex, sizey);
}

void PortalObject::render(Camera* cam)
{
    glm::vec3 diff = (cam->getPosition() - getPosition());
    glm::vec3 ppos = diff + portalb->getPosition();
    pcam->setPosition(ppos.x, ppos.y, ppos.z);

    float newYaw = (getRotation().x + pcam->getYaw()) - portalb->getRotation().x;
    float newPitch = (getRotation().y + pcam->getYaw()) - portalb->getRotation().y;

    pcam->setRotation(newYaw, newPitch);

    fb->bind();
    fb->clear();

    rfunc(pcam);

    if (rb) {
        rb->bind();
    }
    else {
        fb->unbind();
    }

    SceneMeshObject::render(cam);
}

void PortalObject::setSecondPortal(PortalObject* portalb)
{
    this->portalb = portalb;
}

//might be in all renderable objects
void PortalObject::setRenderFrameBuffer(OGLFrameBuffer* rb)
{
    this->rb = rb;
}

void PortalObject::setSceneRenderFunction(void(*r)(Camera* cam))
{
    rfunc = r;
}
