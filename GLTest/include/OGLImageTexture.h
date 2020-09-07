#pragma once
#include <GL/glew.h>
#include <stdlib.h> 
#include <string>
#include "OGLError.h"

class OGLImageTexture
{
public:
	OGLImageTexture(string textPath);
	~OGLImageTexture();
	void bindTexture(unsigned int textureSlot);
private:
	unsigned int textureId;
};