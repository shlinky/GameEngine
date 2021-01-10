#pragma once
#include <GL/glew.h>
#include <iostream>
#include "OGLImageTexture.h"

class OGLFrameBuffer
{
public:
	OGLFrameBuffer();
	~OGLFrameBuffer();
	void attachColorTexture(OGLImageTexture* text);
	void bind();
	void unbind();
private:
	unsigned int fbid;
};

