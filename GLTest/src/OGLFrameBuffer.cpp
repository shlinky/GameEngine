#include "OGLFrameBuffer.h"
#include "..\include\OGLFrameBuffer.h"

//make depth stencil texture assign functions
//make anti aliasing an option
OGLFrameBuffer::OGLFrameBuffer()
{
	glGenFramebuffers(1, &fbid);

	depth_assigned = false;
	numColorTextures = 0;

	width = 0;
	height = 0;
}

OGLFrameBuffer::~OGLFrameBuffer()
{
	glDeleteFramebuffers(1, &fbid);
}

void OGLFrameBuffer::attachColorTexture(OGLImageTexture* text)
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbid);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + numColorTextures, GL_TEXTURE_2D, text->getId(), 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	numColorTextures++;

	if (width == 0) {
		width = text->getWidth();
		height = text->getHeight();
	}
}

void OGLFrameBuffer::createDepthStencilRenderBuffer()
{
	glGenRenderbuffers(1, &depth_stencil_rbid);
	glBindRenderbuffer(GL_RENDERBUFFER, depth_stencil_rbid);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, fbid);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depth_stencil_rbid);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	depth_assigned = true;
}

void OGLFrameBuffer::attachDepthTexture(OGLImageTexture* text)
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbid);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, text->getId(), 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	depth_assigned = true;
}

void OGLFrameBuffer::bind()
{
	if (!depth_assigned) {
		createDepthStencilRenderBuffer();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, fbid);
}

void OGLFrameBuffer::unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OGLFrameBuffer::clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
