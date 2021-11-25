#include "RenderableSceneObject.h"

RenderableSceneObject::RenderableSceneObject()
{
}

void RenderableSceneObject::setHidden(bool hidden)
{
	renderable = !hidden;
}

void RenderableSceneObject::render(Camera* cam)
{
}

void RenderableSceneObject::setHDRRendering(bool HDR)
{
}


