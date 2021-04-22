#pragma once
#include <GL/glew.h>
#include <stdlib.h> 
#include <string>
#include "OGLError.h"

class OGLImageTexture
{
public:
	OGLImageTexture(string textPath);
	OGLImageTexture(unsigned int id, int width = NULL, int height = NULL);
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
	void freeTexture(void* img);
	void loadTextureHDR(string textPath, int* width, int* height, float** img);
	void saveTexture(string path, int w, int h, unsigned char* data);
	int width;
	int height;
};