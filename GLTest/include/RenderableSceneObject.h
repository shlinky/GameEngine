#pragma once
#include "SceneObject.h"
#include "Camera.h"

class RenderableSceneObject: public SceneObject
{
public:
	RenderableSceneObject();
	void setHidden(bool hidden);
	virtual void render(Camera* cam);
	virtual void setHDRRendering(bool HDR);
protected:
	bool HDRRendering = false;

};

