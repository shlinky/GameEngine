#pragma once

#include "SceneMeshObject.h"
#include "OGLFrameBuffer.h"
#include "OGLCubeMapTexture.h"

class HDRCubeMap :
	public OGLCubeMapTexture
{
public:
	HDRCubeMap(string path, int w);
	HDRCubeMap(int w);

	void save(string fname);
	HDRCubeMap* createIrradianceMap(int w = 0);
};