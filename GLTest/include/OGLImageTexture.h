#pragma once
#include <GL/glew.h>
#include <stdlib.h> 
#include <string>
#include "OGLError.h"

class OGLImageTexture
{
public:
	OGLImageTexture(string textPath);
	OGLImageTexture(unsigned int id);
	OGLImageTexture();
	~OGLImageTexture();
	virtual void bindTexture(unsigned int textureSlot = 0);
protected:
	unsigned int textureId;
	void loadTexture(string textPath, int* width, int* height, unsigned char** img);
	void freeTexture(unsigned char* img);
};