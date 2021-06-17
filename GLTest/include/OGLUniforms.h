#pragma once
#include <GL/glew.h>
#include <iostream>
using namespace std;

//Shader Uniforms

class OGLUniform
{
public:
	OGLUniform(string uname, int sId);
	virtual void setUniformData(void* buffer) = 0;
protected:
	void findLocation();
	int uniformLocation;
	int shaderId;
	void* data;
	string name;
};

class OGLUniformMat4FV : public OGLUniform {
public:
	using OGLUniform::OGLUniform;
	void setUniformData(void* buffer);
};

class OGLUniform3FV : public OGLUniform {
public:
	using OGLUniform::OGLUniform;
	void setUniformData(void* buffer);
};

class OGLUniform1I: public OGLUniform {
public:
	using OGLUniform::OGLUniform;
	void setUniformData(void* buffer);
};

class OGLUniformFloat : public OGLUniform {
public:
	using OGLUniform::OGLUniform;
	void setUniformData(void* buffer);
};