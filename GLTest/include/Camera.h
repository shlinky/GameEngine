#pragma once
#include <iostream>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/ext.hpp>
#include <GLM/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "OGLError.h"

//add roll
//maybe make scene object
class Camera
{
public:
	Camera(int* winsizex = nullptr, int* winsizey = nullptr, float* pos = nullptr);
	Camera(float x, float y, float z, int* winsizex = nullptr, int* winsizey = nullptr);
	~Camera();
	void rotateYaw(float degrees);
	void rotatePitch(float degrees);
	void lookAtDir(float x, float y, float z);
	void moveForward(float amount);
	void moveRight(float amount);
	void moveUp(float amount);

	void setPosition(float* pos);
	void setPosition(float x, float y, float z);
	void setRotation(float yaw, float pitch);
	void setSensitivity(float s);
	void setYawLimits(float min, float max);
	void setPitchLimits(float min, float max);

	void getPosition(float* pos);
	glm::vec3 getPosition();
	float getYaw();
	float getPitch();
	glm::mat4 getTransMat(bool motion = true);
	glm::mat4 getTransMatOrtho(bool motion = true);

	void computeVectors();
private:
	glm::vec3 position;
	glm::vec3 upDir;
	glm::vec3 forwardDir;
	glm::vec3 rightDir;
	glm::mat4 viewMat;
	glm::mat4 projMat;
	glm::mat4 projMatOrtho;
	glm::mat4 camTransMat;
	double camYaw;
	double camPitch;
	double sensitivity;
	float yawLim[2];
	float pitchLim[2];
	int* winsizex;
	int* winsizey;
};

