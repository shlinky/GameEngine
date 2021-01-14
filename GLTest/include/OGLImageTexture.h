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
	OGLImageTexture(int width, int height, int textureFormat = GL_RGB);
	OGLImageTexture();
	~OGLImageTexture();
	virtual void bindTexture(unsigned int textureSlot = 0);
	unsigned int getId();
	int getWidth();
	int getHeight();
protected:
	unsigned int textureId;
	void loadTexture(string textPath, int* width, int* height, unsigned char** img);
	void freeTexture(unsigned char* img);
	int width;
	int height;
};