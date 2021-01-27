#define STB_IMAGE_IMPLEMENTATION
#include <STB/stb_image.h>
#include "OGLImageTexture.h"

//create a read and save function
//add a pure virtual base class
OGLImageTexture::OGLImageTexture(string textPath)
{
	unsigned char* img;
	loadTexture(textPath, &width, &height, &img);

	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glGenerateMipmap(GL_TEXTURE_2D);

	freeTexture(img);
	glBindTexture(GL_TEXTURE_2D, 0);
}

OGLImageTexture::OGLImageTexture(unsigned int id)
{
	textureId = id;
	width = NULL;
	height = NULL;
}

OGLImageTexture::OGLImageTexture(int width, int height, int textureFormat)
{
	this->width = width;
	this->height = height;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, textureFormat, width, height, 0, textureFormat, GL_UNSIGNED_BYTE, NULL);
}

OGLImageTexture::OGLImageTexture()
{
	textureId = -1;
	width = NULL;
	height = NULL;
}

OGLImageTexture::~OGLImageTexture()
{
	glDeleteTextures(1, &textureId);
}

void OGLImageTexture::bindTexture(unsigned int textureSlot)
{
	glActiveTexture(GL_TEXTURE0 + textureSlot);
	glBindTexture(GL_TEXTURE_2D, textureId);
}

unsigned int OGLImageTexture::getId()
{
	return textureId;
}

int OGLImageTexture::getWidth()
{
	return width;
}

int OGLImageTexture::getHeight()
{
	return height;
}

void OGLImageTexture::loadTexture(string textPath, int* width, int* height, unsigned char** img)
{
	int _random;
	*img = stbi_load(textPath.c_str(), width, height, &_random, 0);

	if (img == NULL) {
		applicationErrorCallback("Texture path not existant");
	}
}

void OGLImageTexture::freeTexture(unsigned char* img)
{
	stbi_image_free(img);
}
