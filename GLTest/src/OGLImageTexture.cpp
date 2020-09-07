#define STB_IMAGE_IMPLEMENTATION
#include <STB/stb_image.h>
#include "OGLImageTexture.h"

OGLImageTexture::OGLImageTexture(string textPath)
{
	int width, height, _random;
	unsigned char* img = stbi_load(textPath.c_str(), &width, &height, &_random, 0);

	if (img == NULL) {
		applicationErrorCallback("Texture path not existant");
	}

	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);

	stbi_image_free(img);
	glBindTexture(GL_TEXTURE_2D, 0);
}

OGLImageTexture::~OGLImageTexture()
{
	glDeleteTextures(1, &textureId);
}

void OGLImageTexture::bindTexture(unsigned int textureSlot)
{
	glBindTexture(GL_TEXTURE_2D, textureId);
}