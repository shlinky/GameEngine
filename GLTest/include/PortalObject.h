#pragma once
#include "SceneMeshObject.h"
#include "Scene.h"

//add a pointer to the scene so it can render
class PortalObject :
	public SceneMeshObject
{
public:
	PortalObject(int sizex, int sizey);
	void render(Camera* cam);
	void setSecondPortal(PortalObject* portalb);
	PortalObject* getSecondPortal();
	void setScene(Scene* s);
	void captureView();
	void movePortal(glm::vec3 p, glm::quat r);
private:
	OGLFrameBuffer* fb;
	OGLImageTexture* col;
	Camera* pcam;
	PortalObject* portalb;
	Scene* scn;

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
};