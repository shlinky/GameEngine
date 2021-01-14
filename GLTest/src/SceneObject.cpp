#include "SceneObject.h"

SceneObject::SceneObject()
{
}

glm::vec3 SceneObject::getPosition()
{
	return position;
}

glm::vec3 SceneObject::getRotation()
{
	return rotation;
}

glm::vec3 SceneObject::getScale()
{
	return scale;
}

void SceneObject::setPosition(float x, float y, float z)
{
	position = glm::vec3(x, y, z);
}

void SceneObject::setRotation(float x, float y, float z)
{
	rotation = glm::vec3(x, y, z);
}

void SceneObject::setScale(float x, float y, float z)
{
	scale = glm::vec3(x, y, z);
}
