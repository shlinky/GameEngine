#include "Camera.h"

//set rotation
//look at
//get bacl glm types
//changed winsize from pointers to non

//change this constructor into glm::vec3 (maybe get rid of)
Camera::Camera(int winsizex, int winsizey, float* pos)
{
	if (pos != NULL)
		position = glm::make_vec3(pos);
	else
		position = glm::vec3(0.0);

	//setting angles and vecors
	camYaw = glm::radians(-90.0f);
	camPitch = 0.0f;
	forwardDir = glm::vec3(0, 0, -1);
	upDir = glm::vec3(0, 1, 0);
	sensitivity = 1;
	
	//setting angular limits
	yawLim[0] = -1 * (glm::pi<float>() * 2) - 1;
	yawLim[1] = (glm::pi<float>() * 2) + 1;
	pitchLim[0] = -1 * (glm::pi<float>() * 2) - 1;
	pitchLim[1] = (glm::pi<float>() * 2) + 1;
	
	//setting window sizes
	this->winsizex = winsizex;
	this->winsizey = winsizey;

	//make FOV variable
	fov = glm::radians(30.0f);
	nearclip = 0.1f;
	projMat = glm::perspective<float>(fov, (float)winsizex / winsizey, nearclip, 1000.f);
	projMatOrtho = glm::ortho<float>(0.0f, (float)winsizex, 0.0f, (float)winsizey);
	rotation = glm::angleAxis(0.0f, glm::vec3(0, 1, 0));
}

Camera::Camera(float x, float y, float z, int winsizex, int winsizey)
{
	//setting angles and vecors
	position = glm::vec3(x, y, z);
	camYaw = glm::radians(-90.0f);
	camPitch = 0.0f;
	forwardDir = glm::vec3(0, 0, -1);
	upDir = glm::vec3(0, 1, 0);
	sensitivity = 1;

	//setting angular limits
	yawLim[0] = -1 * (glm::pi<float>() * 2) - 1;
	yawLim[1] = (glm::pi<float>() * 2) + 1;
	pitchLim[0] = -1 * (glm::pi<float>() * 2) - 1;
	pitchLim[1] = (glm::pi<float>() * 2) + 1;

	//setting window sizes
	this->winsizex = winsizex;
	this->winsizey = winsizey;

	fov = glm::radians(30.0f);
	nearclip = 0.1f;
	projMat = glm::perspective<float>(fov, (float)winsizex / winsizey, nearclip , 1000.f);
	projMatOrtho = glm::ortho<float>(0.0f, (float)winsizex, 0.0f, (float)winsizey);
	rotation = glm::angleAxis(0.0f, glm::vec3(0, 1, 0));
}

Camera::~Camera()
{
}

void Camera::rotateYaw(float degrees)
{
	float newValue = camYaw + glm::float32(sensitivity) * glm::radians(degrees);
	if ((newValue < yawLim[1]) && (newValue > yawLim[0])) {
		camYaw = fmod(newValue, (glm::pi<float>() * 2));
	}
	rotation = glm::angleAxis((float)camYaw, glm::vec3(0, -1, 0)) * glm::angleAxis((float)camPitch, glm::vec3(1, 0, 0));
	computeVectors();
}

void Camera::rotatePitch(float degrees)
{
	float newValue = camPitch + glm::float32(sensitivity) * glm::radians(degrees);
	if ((newValue < pitchLim[1]) && (newValue > pitchLim[0])) {
		camPitch = fmod(newValue, (glm::pi<float>() * 2));
	}
	rotation = glm::angleAxis((float)camYaw, glm::vec3(0, -1, 0)) * glm::angleAxis((float)camPitch, glm::vec3(1, 0, 0));
	computeVectors();
}

void Camera::lookAtDir(float x, float y, float z)
{
	forwardDir = glm::vec3(x, y, z);
	rightDir = glm::cross(forwardDir, upDir);

	//calculating angles
	double leg = sqrt(pow(forwardDir.x, 2) + pow(forwardDir.z, 2));
	camPitch = atan(forwardDir.y / leg);

	camYaw = atan(forwardDir.x / forwardDir.z);

	//cout << abs((camPitch - glm::radians(90.0f))) << endl;
	if (abs((abs(camPitch) - glm::radians(90.0f))) < 0.1) {
		upDir = glm::vec3(0, 0, 1);
	}
	else {
		upDir = glm::vec3(0, 1, 0);
	}
}

float Camera::getYaw()
{
	return glm::degrees(camYaw);
}

float Camera::getPitch()
{
	return glm::degrees(camPitch);
}

float Camera::getRoll()
{
	return camRoll;
}

void Camera::moveForward(float amount)
{
	glm::float32 multiplier(amount);
	position += multiplier * forwardDir;
}

void Camera::moveRight(float amount)
{
	glm::float32 multiplier(amount);
	position += multiplier * rightDir;
}

void Camera::moveUp(float amount)
{
	glm::float32 multiplier(amount);
	position += multiplier * upDir;
}


void Camera::setPosition(float* pos)
{
	position.x = pos[0];
	position.y = pos[1];
	position.z = pos[2];
}

void Camera::setPosition(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;
}

void Camera::setRotation(float yaw, float pitch)
{
	this->camYaw = glm::radians(yaw);
	this->camPitch = glm::radians(pitch);
	rotation = glm::angleAxis((float)camYaw, glm::vec3(0, 1, 0)) * glm::angleAxis((float)camPitch, glm::vec3(1, 0, 0));
	computeVectors();
}

void Camera::setUpDir(float x, float y, float z)
{
	upDir = glm::vec3(x, y, z);
}

void Camera::getPosition(float* pos)
{
	pos[0] = position.x;
	pos[1] = position.y;
	pos[2] = position.z;
}

glm::vec3 Camera::getPosition()
{
	return position;
}

glm::mat4 Camera::getTransMat(bool motion)
{
	viewMat = glm::lookAt(
		position,
		position + forwardDir, 
		upDir
	);
	if (motion) {
		camTransMat = projMat * viewMat;
	}
	else {
		camTransMat = projMat * glm::mat4(glm::mat3(viewMat));
	}
	return camTransMat;
}

glm::mat4  Camera::getTransMatOrtho(bool motion)
{
	viewMat = glm::lookAt(
		position,
		position + forwardDir,
		upDir
	);
	if (motion) {
		camTransMat = projMatOrtho * viewMat;
	}
	else {
		camTransMat = projMatOrtho * glm::mat4(glm::mat3(viewMat));
	}
	return camTransMat;
}

glm::vec3 Camera::getForwardDir()
{
	return forwardDir;
}

glm::vec3 Camera::getRightDir()
{
	return rightDir;
}

glm::quat Camera::getQuatRotation()
{
	return rotation;
}

void Camera::computeVectors()
{
	/*forwardDir.x = cos(camYaw) * cos(camPitch);
	forwardDir.y = sin(camPitch);
	forwardDir.z = sin(camYaw) * cos(camPitch);
	rightDir = glm::normalize(glm::cross(forwardDir, upDir));*/
	forwardDir = rotation * glm::vec3(0, 0, -1);
	rightDir = rotation * glm::vec3(1, 0, 0);
	upDir = rotation * glm::vec3(0, 1, 0);
	//cout << glm::to_string(forwardDir) << ' ' << glm::to_string(upDir) << endl;
	camYaw = atan2(glm::dot(rightDir, glm::vec3(1, 0, 0)), glm::dot(rightDir, glm::vec3(0, 0, 1)));
	camPitch = atan2(glm::dot(upDir, glm::cross(rightDir, glm::vec3(0, 1, 0))), glm::dot(upDir, glm::vec3(0, 1, 0)));
	camRoll = atan2(glm::dot(rightDir, glm::cross(glm::cross(forwardDir, glm::vec3(0, 1, 0)), forwardDir)), glm::dot(rightDir, glm::cross(forwardDir, glm::vec3(0, 1, 0))));
	//cout << "gucci " << to_string(forwardDir) << ' ' << camRoll << endl;
}

void Camera::setSensitivity(float s)
{
	sensitivity = s;
}

void Camera::setYawLimits(float min, float max)
{
	yawLim[0] = glm::radians(min);
	yawLim[1] = glm::radians(max);
}

void Camera::setPitchLimits(float min, float max)
{
	pitchLim[0] = glm::radians(min);
	pitchLim[1] = glm::radians(max);
}

void Camera::setFOV(float fov)
{
	this->fov = glm::radians(fov);
	projMat = glm::perspective<float>(this->fov, (float)winsizex / winsizey, nearclip, 1000.f);
}

void Camera::setQuatRotation(glm::quat q)
{
	rotation = q;
	//glm::vec3 a = glm::eulerAngles(rotation);
	//camYaw = a.y;
	//camPitch = a.x;
	computeVectors();
}

void Camera::setNearClip(float d)
{
	nearclip = d;
	projMat = glm::perspective<float>(fov, (float)winsizex / winsizey, nearclip, 1000.f);
}

