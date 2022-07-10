#include "PortalObject.h"
#include <string>

PortalObject::PortalObject(int sizex, int sizey)
{
    OGLVertexObject* mesh = new OGLVertexObject(4);
	mesh->addAttribute(0, 3, screenvp);
	mesh->addIndexing(screenindex, 6);
    setMesh(mesh);

    fb = new OGLFrameBuffer();
    col = new OGLImageTexture(sizex, sizey, true);
    altCol = new OGLImageTexture(sizex, sizey, true);
    fb->attachColorTexture(col);
    currBoundTexture = col;

    createShader("res/shaders/p.vert", "res/shaders/p.frag");
    getShader()->addTexture(col);
    getShader()->addTexture("res/shaders/frame.png");

    pcam = new Camera(0, 0, 0, sizex, sizey);
    setRotation(0, 0, 0);
}

void PortalObject::render(Camera* cam)
{
    //currBoundTexture->save("portalt" + std::to_string(id) + ".jpg");

    this->SceneMeshObject::render(cam);
}

void PortalObject::setSecondPortal(PortalObject* portalb)
{
    this->portalb = portalb;
}

PortalObject* PortalObject::getSecondPortal()
{
    return portalb;
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
    glm::vec3 cpos = cam->getPosition();
    pcam->setPosition(cpos.x, cpos.y, cpos.z);
    pcam->setQuatRotation(cam->getQuatRotation());
    teleport(pcam);


    //rendering into portal texture
    currBoundTexture = (currBoundTexture == col) ? altCol : col;
    fb->bind();
    //fb->clear();
    fb->resetColorTextures();
    fb->attachColorTexture(currBoundTexture);
    fb->bind();
    fb->clear();
    fb->unbind();

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

    portalb->setHidden(true);
    OGLFrameBuffer* ob = scn->getFinalBuffer();

    scn->setRenderBuffer(fb);
    scn->renderHDR(true);
    scn->setCamera(pcam);
    scn->render();

    scn->setCamera(cam);
    scn->renderHDR(false);
    scn->setRenderBuffer(ob);
    portalb->setHidden(false);

    p = 0.0f;
    for (int i = 0; i < scn->getObjectCount(); i++) {
        SceneObject* o = scn->getObject(i);
        if (o->getEditorMovable()) {
            SceneMeshObject* m = (SceneMeshObject*)o;
            m->getShader()->updateUniformData("isPortalCapture", &p);
        }
    }

    getShader()->changeTexture(currBoundTexture, 0);
}


void PortalObject::movePortal(glm::vec3 p, glm::quat r)
{
    glm::vec3 up = glm::toMat3(r) * glm::vec3(0, 1, 0);
    glm::vec3 right = glm::toMat3(r) * glm::vec3(1, 0, 0);

    glm::vec3 camUp = glm::cross(scn->getCamera()->getRightDir(), glm::vec3(0, 0, -1));
    glm::vec2 portalSurfaceRotation = glm::normalize(glm::vec2(glm::dot(camUp, right), glm::dot(camUp, up)));

    float sTheta = atan2(portalSurfaceRotation.y, portalSurfaceRotation.x);

    glm::vec3 sNormal = glm::toMat3(r) * glm::vec3(0, 0, 1);
    glm::quat sRot = glm::angleAxis(sTheta - glm::radians(90.0f), sNormal);
    glm::quat portalFinalRot = sRot * r;

    glm::vec3 finalP = p + (0.05f * sNormal);
    setPosition(finalP.x, finalP.y, finalP.z);
    rotation = portalFinalRot;
}

//its a question of making sure the whole collision object is inside the border of the portal, so that it is not colliding with anything else
//it should be not just above bt should have multiple functions for different collision types and return wheter it is completely within borders
//withinPortalBorders
bool PortalObject::abovePortal(glm::vec3 pos)
{
    glm::vec3 portalDir = glm::normalize(glm::toMat3(getQuatRotation()) * glm::vec3(0, 0, 1));
    if (portalDir.y <= 0) return false;

    glm::vec3 portalOffset = pos - getPosition();
    glm::vec2 pOffFlat = glm::vec2(portalOffset.x, portalOffset.z);
    glm::vec3 rightDir = glm::toMat3(getQuatRotation()) * glm::vec3(1, 0, 0);
    glm::vec3 upDir = glm::toMat3(getQuatRotation()) * glm::vec3(0, 1, 0);
    glm::vec2 rightFlat = glm::vec2(rightDir.x, rightDir.z);
    glm::vec2 upFlat = glm::vec2(upDir.x, upDir.z);

    glm::vec2 hOffsets = glm::vec2(dot(rightFlat, pOffFlat) / pow(glm::length(rightFlat), 2), dot(upFlat, pOffFlat) / pow(glm::length(upFlat), 2));

    if ((fabs(hOffsets.x) > 2.5) || (fabs(hOffsets.y) > 5)) return false;
    float portalY = rightDir.y * hOffsets.x + upDir.y * hOffsets.y;

    if (portalY > pos.y) return false;
    return true;
}

void PortalObject::transform_vector_portal(glm::vec3& v)
{
    v = v * glm::toMat3(glm::normalize(getQuatRotation()));
    v.x *= -1;
    v.z *= -1;
    v = glm::toMat3(getSecondPortal()->getQuatWorldRotation()) * v;
}

void PortalObject::teleport(Camera* c)
{
    PortalObject* p2 = getSecondPortal();
    glm::vec3 diff = (c->getPosition() - getPosition());

    transform_vector_portal(diff);

    glm::vec3 ppos = diff + p2->getPosition();
    c->setPosition(ppos.x, ppos.y, ppos.z);

    glm::quat oQuat = c->getQuatRotation();
    oQuat = glm::angleAxis(glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0)) * (glm::normalize(glm::inverse(getQuatRotation())) * oQuat);
    oQuat = p2->getQuatWorldRotation() * oQuat;
    c->setQuatRotation(oQuat);
}

bool PortalObject::enteredPortal(glm::vec3 pos, glm::vec3 ppos)
{
    glm::vec3 portalOffset = pos - getPosition();
    glm::vec3 prevPortalOffset = ppos - getPosition();
    glm::vec3 portalDir = glm::normalize(glm::toMat3(getQuatRotation()) * glm::vec3(0, 0, 1));
    float distFromPortal = glm::dot(portalDir, portalOffset);
    float prevDistFromPortal = glm::dot(portalDir, prevPortalOffset);

    if (!((distFromPortal < 0) && (prevDistFromPortal > 0))) return false;

    glm::vec3 rightDir = glm::toMat3(getQuatRotation()) * glm::vec3(1, 0, 0);
    glm::vec3 upDir = glm::toMat3(getQuatRotation()) * glm::vec3(0, 1, 0);

    glm::vec2 xyoff = glm::vec2(glm::dot(rightDir, portalOffset), glm::dot(upDir, portalOffset));

    if ((fabs(xyoff.x) > 3) || (fabs(xyoff.y) > 6)) return false;

    return true;
}