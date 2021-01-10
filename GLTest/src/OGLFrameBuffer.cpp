#include "OGLFrameBuffer.h"
#include "..\include\OGLFrameBuffer.h"

OGLFrameBuffer::OGLFrameBuffer()
{
	glGenFramebuffers(1, &fbid);
}

OGLFrameBuffer::~OGLFrameBuffer()
{
	glDeleteFramebuffers(1, &fbid);
}

void OGLFrameBuffer::attachColorTexture(OGLImageTexture* text)
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbid);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, text->getId(), 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OGLFrameBuffer::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbid);
}

void OGLFrameBuffer::unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
