#pragma once
#include "OGLShaderProgram.h"
#include "OGLImageTexture.h"
#include "OGLCubeMapTexture.h"

class OGLTexturedShader :
	public OGLShaderProgram
{
public:
	OGLTexturedShader(string vs, string fs, int numUniforms = 0, int numTextures = 0);
	~OGLTexturedShader();
	void addTexture(string tName);
	void addTexture(OGLImageTexture* texture);
	void addCubemapTexture(string* tNames);
	void bindShaderProgram();
private:
	int numTextures;
	OGLImageTexture** textures;
	int texturesAdded;
};