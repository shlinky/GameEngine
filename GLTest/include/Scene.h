#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <iomanip>
#include <string>
//#include "OGLError.h"
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
//change vertex shader for uv rendering
class Scene
{
public:
	Scene(WindowsWindowing* win);
	void addSceneObject(SceneObject* obj);
	void render();
	void renderWithEditorFunctionality();
	SceneObject* getMouseTrace(float x, float y);
	void applyObjectEdits(float x, float y, float x1, float y1);
	void selectObjManipulator(float x, float y);
	void setSelectedObject(int id);
	SceneObject* getObject(int id);
	Camera* getCamera();
	int getObjectCount();
	void setCamera(Camera* cam);

	//maybe make it automatically render with functionality if this flag is set to true, in other words else in the function
	void setEditorFunctionality(bool on);

	void setRenderBuffer(OGLFrameBuffer* rb);
	
	//gives the scene buffer for objects to render onto
	OGLFrameBuffer* getRenderBuffer();
	OGLFrameBuffer* getFinalBuffer();

	//change this to render with post
	void renderHDR(bool on);
private:
	//maybe make into non pointers, but this will require no parameter constructors for these classes
	vector<SceneObject*> sceneObjects;
	OGLFrameBuffer* sceneBuffer;
	//make it so that the render is called by render engine func and it sets final buffer to another buffer
	OGLFrameBuffer* finalBuffer = nullptr;
	OGLFrameBuffer* outlineBuffer;
	OGLFrameBuffer preOverlayBuffer;

	OGLImageTexture* preOverlayTexture;
	OGLImageTexture* renderTexture;
	OGLImageTexture* colorIDTexture;
	OGLImageTexture* arrowTexture;
	OGLImageTexture* outlineTexture;
	OGLImageTexture* blank;

	SceneObject* selectedObject;

	
	//final buffer

	//make this into separate class for screen space elements
	OGLVertexObject* screenquad;
	OGLTexturedShader* postprocess;
	OGLTexturedShader* eprocess;

	SceneMeshObject arrows[3];
	SceneMeshObject objManipulator;
	SceneMeshObject* selectedArrow;

	Camera* renderCam;
	//colorids flag that is turned on when editorfunc is turned on
	bool editorFunctionality = false;
};

