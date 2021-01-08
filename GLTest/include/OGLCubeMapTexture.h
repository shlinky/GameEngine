#pragma once
#include "OGLImageTexture.h"

class OGLCubeMapTexture: 
	public OGLImageTexture
{
public:
	OGLCubeMapTexture(string* texturePaths);
	void bindTexture(unsigned int textureSlot = 0);
};

