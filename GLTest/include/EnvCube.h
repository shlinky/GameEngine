#pragma once
#include "SceneObject.h"
#include "OGLCubeMapTexture.h"

class EnvCube :
	public SceneObject
{
public:
	EnvCube(OGLCubeMapTexture* cm);
	void render(Camera* cam);
private:
	OGLCubeMapTexture* cubeMap;
	OGLVertexObject* cube;
	OGLTexturedShader* shader;
};

