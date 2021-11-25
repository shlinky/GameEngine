#pragma once
#include "SceneObject.h"
#include  "RenderableSceneObject.h"
#include "OGLCubeMapTexture.h"

//Environment Cube SceneObject
class EnvCube :
	public RenderableSceneObject
{
public:
	EnvCube(OGLCubeMapTexture* cm);
	void render(Camera* cam);
	void setCubeMap(OGLCubeMapTexture* cm);
	void setHDRRendering(bool hdr);
private:
	OGLCubeMapTexture* cubeMap;
	OGLVertexObject* cube;
	OGLTexturedShader* shader;
};

