#pragma once
#include "SceneObject.h"
#include "OGLCubeMapTexture.h"

//Environment Cube SceneObject
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

