#include "OGLUniforms.h"
#include "..\include\OGLUniforms.h"

OGLUniform::OGLUniform(string uname, int sId)
{
	this->name = uname;
	shaderId = sId;
	findLocation();
}

void OGLUniform::findLocation()
{
	uniformLocation = glGetUniformLocation(shaderId, name.c_str());
}

void OGLUniformMat4FV::setUniformData(void* buffer)
{
	glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, (float*)buffer);
	data = buffer;
}

void OGLUniform3FV::setUniformData(void* buffer)
{
	glUniform3fv(uniformLocation, 1, (float*)(buffer));
	data = buffer;
}
