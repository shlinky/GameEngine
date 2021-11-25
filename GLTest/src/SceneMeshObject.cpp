#include "SceneMeshObject.h"

#include <Windows.h>

SceneMeshObject::SceneMeshObject()
{
	setPosition(0, 0, 0);
	setScale(1, 1, 1);
	setScale(0, 0, 0);
	renderable = true;
	editorMovableObject = true;
}

SceneMeshObject::SceneMeshObject(string model)
{
	mesh = new OGLVertexObject(model, true, 4);
	setPosition(0, 0, 0);
	setScale(1, 1, 1);
	setScale(0, 0, 0);
	renderable = true;
	editorMovableObject = true;
}

SceneMeshObject::SceneMeshObject(OGLVertexObject* model)
{
	mesh = model;
	setPosition(0, 0, 0);
	setScale(1, 1, 1);
	setRotation(0, 0, 0);
	renderable = true;
	editorMovableObject = true;
}

SceneMeshObject::SceneMeshObject(float x, float y, float z)
{
	setPosition(x, y, z);
	setScale(1, 1, 1);
	setRotation(0, 0, 0);
	renderable = true;
	editorMovableObject = true;
}

SceneMeshObject::SceneMeshObject(string model, float x, float y, float z)
{
	mesh = new OGLVertexObject(model, true, 4);
	setPosition(x, y, z);
	setScale(1, 1, 1);
	setRotation(0, 0, 0);
	renderable = true;
	editorMovableObject = true;
}

void SceneMeshObject::setMesh(OGLVertexObject* model)
{
	mesh = model;
}

void SceneMeshObject::setMesh(string model)
{
	mesh = new OGLVertexObject(model, true, 4);
}

void SceneMeshObject::setShader(OGLTexturedShader* shader)
{
	this->shader = shader;
}

void SceneMeshObject::createShader(string vs, string fs, int numUniforms, int numTextures)
{
	shader = new OGLTexturedShader(vs, fs, numUniforms + 4, numTextures);
	shader->addUniform<OGLUniformMat4FV>("mvp");
	shader->addUniform<OGLUniform3FV>("camera_position");
	shader->addUniform<OGLUniformMat4FV>("world");
	shader->addUniform<OGLUniformFloat>("HDR");
	
	float hdrOn = 0.0f;
	shader->updateUniformData("HDR", &hdrOn);
}

OGLTexturedShader* SceneMeshObject::getShader()
{
	return shader;
}

OGLVertexObject* SceneMeshObject::getMesh()
{
	return mesh;
}

//with framebuffer
void SceneMeshObject::render(Camera* cam)
{
	//creating mvp matrix
	glm::mat4 vp = cam->getTransMat();
	glm::mat4 modelmat = generateModelMat();
	mvp = vp * modelmat;

	glm::vec3 campos = cam->getPosition();

	shader->updateUniformData("mvp", &mvp[0][0]);
	shader->updateUniformData("camera_position", &campos);
	shader->updateUniformData("world", &modelmat);

	bind();
	glDrawElements(GL_TRIANGLES, mesh->getIndexCount(), GL_UNSIGNED_INT, (void*)0);
}

void SceneMeshObject::bind()
{
	mesh->bind();
	shader->bindShaderProgram();
}

void SceneMeshObject::setDepthTest(bool depth)
{
	depthtest = depth;
}

void SceneMeshObject::setHDRRendering(bool HDR)
{
	HDRRendering = HDR;
	float hbool;
	hbool = (float)(HDR ? 1 : 0);
	shader->updateUniformData("HDR", &hbool);
}
