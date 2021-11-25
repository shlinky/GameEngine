#pragma once
#include "OGLShaderProgram.h"
#include "OGLImageTexture.h"
#include "OGLCubeMapTexture.h"

//Shader that also handles references to textures associated with that shader

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
	void changeTexture(OGLImageTexture* texture, int index);
private:
	int numTextures;
	OGLImageTexture** textures;
	int texturesAdded;
};