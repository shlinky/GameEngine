#include "SceneMeshObject.h"
#include "..\include\SceneMeshObject.h"

SceneMeshObject::SceneMeshObject()
{
	renderable = true;
	position = glm::vec3(0, 0, 0);
	scale = glm::vec3(1, 1, 1);
	rotation = glm::vec3(0, 0, 0);
}

SceneMeshObject::SceneMeshObject(string model)
{
	mesh = new OGLVertexObject(model, true, 4);
	position = glm::vec3(0, 0, 0);
	scale = glm::vec3(1, 1, 1);
	rotation = glm::vec3(0, 0, 0);
}

SceneMeshObject::SceneMeshObject(OGLVertexObject* model)
{
	mesh = model;
	position = glm::vec3(0, 0, 0);
	scale = glm::vec3(1, 1, 1);
	rotation = glm::vec3(0, 0, 0);
}

SceneMeshObject::SceneMeshObject(float x, float y, float z)
{
	position = glm::vec3(x, y, z);
	scale = glm::vec3(1, 1, 1);
	rotation = glm::vec3(0, 0, 0);
}

SceneMeshObject::SceneMeshObject(string model, float x, float y, float z)
{
	mesh = new OGLVertexObject(model, true, 4);
	position = glm::vec3(x, y, z);
	scale = glm::vec3(1, 1, 1);
	rotation = glm::vec3(0, 0, 0);
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
	shader = new OGLTexturedShader(vs, fs, numUniforms, numTextures);
	shader->addUniform<OGLUniformMat4FV>("mvp");
	shader->addUniform<OGLUniform3FV>("camera_position");
	shader->addUniform<OGLUniformMat4FV>("world");
}

OGLTexturedShader* SceneMeshObject::getShader()
{
	return shader;
}

OGLVertexObject* SceneMeshObject::getMesh()
{
	return mesh;
}

void SceneMeshObject::render(Camera* cam)
{
	//creating mvp matrix
	glm::mat4 vp = cam->getTransMat();
	glm::mat4 modelmat = glm::scale(scale);
	modelmat = glm::rotate(rotation.z, glm::vec3(0, 0, 1)) 
		* glm::rotate(rotation.y, glm::vec3(0, 1, 0)) 
		* glm::rotate(rotation.x, glm::vec3(1, 0, 0)) * modelmat;
	modelmat = glm::translate(position) * modelmat;
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
