#pragma once
#include "SceneMeshObject.h"

//add a pointer to the scene so it can render
class PortalObject :
	public SceneMeshObject
{
public:
	PortalObject(float x, float y, float z, int sizex, int sizey);
	void render(Camera* cam);
	void setSecondPortal(PortalObject* portalb);
	void setRenderFrameBuffer(OGLFrameBuffer* rb);
	void setSceneRenderFunction(void (*r)(Camera* cam));
private:
	OGLFrameBuffer* fb;
	Camera* pcam;
	PortalObject* portalb;
	OGLFrameBuffer* rb;
	void (*rfunc)(Camera* cam);

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