#pragma once

#include <GL/glew.h>
#include <iostream>
#include "OGLImageTexture.h"
#include "OGLCubeMapTexture.h"

//store pointers to textures
class OGLFrameBuffer
{
public:
	OGLFrameBuffer();
	~OGLFrameBuffer();
	void attachColorTexture(OGLImageTexture* text);
	void createDepthStencilRenderBuffer();
	void attachDepthTexture(OGLImageTexture* text);
	void attachColorTextureCM(OGLCubeMapTexture* text, int face, int mip = 0);
	void bind();
	void unbind();
	void clear();
	void resetColorTextures();
	//for resizing viewport after unbinding
private:
	unsigned int fbid;
	unsigned int depth_stencil_rbid;
	bool depth_assigned;
	int width;
	int height;
	int defWidth;
	int defHeight;

	int numColorTextures;
};

