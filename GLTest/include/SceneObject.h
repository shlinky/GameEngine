#pragma once
#include <GL/glew.h>
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
#include <glm/gtc/quaternion.hpp> 
#include <glm/gtx/quaternion.hpp>


class SceneObject
{
public:
	SceneObject();
	glm::vec3 getPosition();
	glm::vec3 getRotation();
	glm::quat getQuatRotation();
	glm::vec3 getWorldRotation();
	glm::quat getQuatWorldRotation();
	glm::vec3 getScale();
	void setPosition(float x, float y, float z);
	void setRotation(float x, float y, float z);
	void setScale(float x, float y, float z);
	void Rotate(float x, float y, float z);
	static glm::vec3 Rotate(glm::vec3 r1, glm::vec3 r2);

	glm::mat4 generateModelMat();

	void setIsComponent(bool isComp);
	void setParent(SceneObject* parent);
	
	void setId(int eid);
	int getId();
protected:
	glm::vec3 position;
	glm::vec3 scale;
	glm::quat rotation;

	bool isComponent;
	SceneObject* parentObject;
	int id = -1;
	//properties hash table
};

