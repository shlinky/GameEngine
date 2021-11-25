#pragma once

#include "OGLImageTexture.h"

class SceneMeshObject;
class OGLFrameBuffer;
class Scene;

class OGLCubeMapTexture: 
	public OGLImageTexture
{
public:
	OGLCubeMapTexture();
	OGLCubeMapTexture(string* texturePaths);
	OGLCubeMapTexture(string path, int w);
	OGLCubeMapTexture(int w, bool mipmap = false);
	void renderIntoCubemap(Scene* scn);

	void save(string fname);
	OGLCubeMapTexture* createIrradianceMap(int w, OGLCubeMapTexture* cm = nullptr);
	OGLCubeMapTexture* createPrefilteredSpec(int w, OGLCubeMapTexture* cm = nullptr);

	//OGLCubeMapTexture(int w);
	void bindTexture(unsigned int textureSlot = 0);
};

