#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <iomanip>
#include <string>
#include "OGLError.h"
#include "OGLShaderProgram.h"
#include "OGLVertexObject.h"
#include "WindowsWindowing.h"
#include "OGLTexturedShader.h"
#include "OGLFrameBuffer.h"
#include "Camera.h"
#include "ModelLoader.h"
#include <math.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/ext.hpp>
#include <GLM/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include "SceneMeshObject.h"
#include "EnvCube.h"
#include "Windows.h"
#include <vector>

//every object has names and ids maybe (only assigned an id when in a scene always has a name)
//stores all settings for rendering a specific scene
class Scene
{
public:
	Scene(WindowsWindowing* win);
	void addSceneObject(SceneObject* obj);
	void render();
	void renderWithEditorFunctionality();
	SceneObject* getMouseTrace(float x, float y);
	void applyObjectEdits(float x, float y, float x1, float y1);
	SceneObject* getObject(int id);
	Camera* getCamera();

	//maybe make it automatically render with functionality if this flag is set to true
	void setEditorFunctionality(bool on);
private:
	//maybe make into non pointers, but this will require no parameter constructors for these classes
	vector<SceneObject*> sceneObjects;
	OGLFrameBuffer* mainBuffer;
	OGLFrameBuffer* outlineBuffer;
	OGLImageTexture renderTexture;
	OGLImageTexture colorIDTexture;
	OGLImageTexture arrowTexture;
	OGLImageTexture outlineTexture;
	SceneObject* selectedObject;

	//make this into separate class for screen space elements
	OGLVertexObject* screenquad;
	OGLTexturedShader* postprocess;

	SceneMeshObject* arrows[3];
	SceneMeshObject* objManipulator;

	Camera* renderCam;
	bool editorFunctionality = false;
};

