#pragma once
#include "SceneObject.h"
#include "RenderableSceneObject.h"
//Mesh Objects that are in the scene (have pos rot scale)

//hidden objects
//inherits from renderable scene object
//has outline option
class SceneMeshObject :
	public RenderableSceneObject
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
	void bind();

	void setDepthTest(bool depth);
	void setHDRRendering(bool HDR);
private:
	OGLVertexObject* mesh;
	OGLTexturedShader* shader;
	glm::mat4 mvp;
	bool depthtest = true;
};

