#include "OGLTexturedShader.h"

OGLTexturedShader::OGLTexturedShader(string vs, string fs, int numUniforms, int numTextures): OGLShaderProgram(vs, fs, numUniforms)
{
	this->numTextures = numTextures;
	textures = new OGLImageTexture*[numTextures];
	texturesAdded = 0;
}

//make parent virtual maybe
OGLTexturedShader::~OGLTexturedShader()
{
	OGLShaderProgram::~OGLShaderProgram();
	delete[] textures;
}

void OGLTexturedShader::addTexture(string tName)
{
	if (texturesAdded == numTextures) {
		applicationErrorCallback("No more textures available, already added max amount");
	}
	textures[texturesAdded] = new OGLImageTexture(tName);
	texturesAdded++;
}

void OGLTexturedShader::addTexture(OGLImageTexture* texture)
{
	if (texturesAdded == numTextures) {
		applicationErrorCallback("No more textures available, already added max amount");
	}
	textures[texturesAdded] = texture;
	texturesAdded++;
}

void OGLTexturedShader::addCubemapTexture(string* tNames)
{
	if (texturesAdded == numTextures) {
		applicationErrorCallback("No more textures available, already added max amount");
	}
	textures[texturesAdded] = new OGLCubeMapTexture(tNames);
	texturesAdded++;
}

void OGLTexturedShader::bindShaderProgram()
{
	OGLShaderProgram::bindShaderProgram();
	for (int i = 0; i < texturesAdded; i++) {
		textures[i]->bindTexture(i);
	}
}
