#pragma once
#include <GL/glew.h>
#include <iostream>
#include "OGLImageTexture.h"

//store pointers to textures
class OGLFrameBuffer
{
public:
	OGLFrameBuffer();
	~OGLFrameBuffer();
	void attachColorTexture(OGLImageTexture* text);
	void createDepthStencilRenderBuffer();
	void attachDepthTexture(OGLImageTexture* text);
	void bind();
	void unbind();
	void clear();
private:
	unsigned int fbid;
	unsigned int depth_stencil_rbid;
	bool depth_assigned;
	int width;
	int height;

	int numColorTextures;
};

