#pragma once
#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <stdlib.h> 
#include <unordered_map>
#include <string> 
#include "OGLUniforms.h"
#include "OGLError.h"
using namespace std;

class OGLShaderProgram
{
public:
	OGLShaderProgram(string vs, string fs, int numUniforms = 0);
	~OGLShaderProgram();
	void bindShaderProgram();
	template<typename U> void addUniform(string name, void* buffer = nullptr);
	void updateUniformData(string name, void* buffer);
private:
	unsigned int CreateShaderProgram(string vs, string fs);
	unsigned int CreateShader(char* source, unsigned int type);
	void readShaderFile(string fname, char** contents);
	unsigned int pid;
	OGLUniform** uniforms;
	int uniformsAdded;
	unordered_map<string, int> uniformNameToLocation;
	int numUniforms;
};

template<typename U> void OGLShaderProgram::addUniform(string uName, void* buffer)
{
	if (!is_base_of<OGLUniform, U>::value) {
		applicationErrorCallback("Uniform type supplied is not a child of OGLUniform");
	}
	if (uniformsAdded >= numUniforms) {
		applicationErrorCallback("You have initialized shader with max uniforms of " + to_string(numUniforms) + ", and are attempting to add more than this limit");
	}

	glUseProgram(pid);

	uniforms[uniformsAdded] = new U(uName, pid);
	uniformNameToLocation[uName] = uniformsAdded;
	if (buffer != NULL) {
		uniforms[uniformsAdded]->setUniformData(buffer);
	}
	uniformsAdded++;

	glUseProgram(0);
}