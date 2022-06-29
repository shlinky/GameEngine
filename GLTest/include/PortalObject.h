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
	bool enteredPortal(glm::vec3 pos, glm::vec3 ppos);
	bool abovePortal(glm::vec3 pos);
	void transform_vector_portal(glm::vec3& v);

	//make scene object as the parameter in addition to camera within the portalobject class
	//make portalable scene object class that contains code for duplication and culling when within a portal
	void teleport(Camera* c);

private:
	OGLFrameBuffer* fb;
	OGLImageTexture* col;

	//for the portal infinite effect
	OGLImageTexture* altCol;
	OGLImageTexture* currBoundTexture;

	Camera* pcam;
	PortalObject* portalb;
	Scene* scn;

	float screenvp[12] = {
		1, -2, 0,
		1,  2, 0,
	   -1,  2, 0,
	   -1, -2, 0,
	};

	unsigned int screenindex[6] = {
		0, 1, 2,
		0, 3, 2
	};
};