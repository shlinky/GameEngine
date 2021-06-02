#pragma once

#include "SceneObject.h"

//hidden objects
//inherits from renderable scene object
class SceneMeshObject :
	public SceneObject
{
public:
	SceneMeshObject();
	SceneMeshObject(string model);
	SceneMeshObject(OGLVertexObject* model);
	SceneMeshObject(float x, float y, float z);
	SceneMeshObject(string model, float x, float y, float z);

	void setMesh(OGLVertexObject* model);
	void setMesh(string model);
	void setShader(OGLTexturedShader* shader);
	void createShader(string vs, string fs, int numUniforms = 5, int numTextures = 4);

	OGLTexturedShader* getShader();
	OGLVertexObject* getMesh();

	void render(Camera* cam);
	void trender(Camera* cam);
	void bind();
private:
	OGLVertexObject* mesh;
	OGLTexturedShader* shader;
	glm::mat4 mvp;
};

