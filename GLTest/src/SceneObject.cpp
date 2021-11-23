#include "SceneObject.h"

//components
//get is component
//get world position rotation scale for components
SceneObject::SceneObject()
{
	isComponent = false;
	scale = glm::vec3(1, 1, 1);
}

glm::vec3 SceneObject::getPosition()
{
	return position;
}

glm::vec3 SceneObject::getRotation()
{
	glm::vec3 pyl = glm::degrees(glm::eulerAngles(rotation));
	return (glm::vec3(pyl.y, pyl.x, pyl.z));
}

glm::quat SceneObject::getQuatRotation()
{
	return rotation;
}

//remove these non quaternion functions instead implement a converter
glm::vec3 SceneObject::getWorldRotation()
{
	if (isComponent) {
		glm::quat wRot = (parentObject->getQuatWorldRotation() * rotation);
		glm::vec3 pyl = glm::degrees(glm::eulerAngles(rotation));
		return (glm::vec3(pyl.y, pyl.x, pyl.z));
	}
	else {
		return (getRotation());
	}
}

glm::quat SceneObject::getQuatWorldRotation()
{
	if (isComponent) {
		//cout << "parent rotation " << glm::to_string((glm::normalize(parentObject->getQuatWorldRotation()) * glm::normalize(rotation))) << endl;
		return ((glm::normalize(parentObject->getQuatWorldRotation()) * glm::normalize(rotation)));
	}
	else {
		return (rotation);
	}
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
	glm::quat QuatAroundX = glm::angleAxis(glm::radians(x), glm::vec3(1.0, 0.0, 0.0));
	glm::quat QuatAroundY = glm::angleAxis(glm::radians(y), glm::vec3(0.0, 1.0, 0.0));
	glm::quat QuatAroundZ = glm::angleAxis(glm::radians(z), glm::vec3(0.0, 0.0, 1.0));
	rotation = QuatAroundZ * QuatAroundY * QuatAroundX;
	//cout << glm::to_string(glm::eulerAngles(rotation)) << endl;
}

void SceneObject::setScale(float x, float y, float z)
{
	scale = glm::vec3(x, y, z);
}

void SceneObject::Rotate(float x, float y, float z)
{
	glm::quat newrot = glm::quat(glm::vec3(x, y, z));
	rotation = newrot * rotation;
}

glm::vec3 SceneObject::Rotate(glm::vec3 r1, glm::vec3 r2)
{
	glm::quat newrot = glm::quat(glm::radians(r2));
	glm::quat r = glm::normalize(newrot * glm::quat(glm::radians(r1)));
	glm::vec3 pyl = glm::eulerAngles(r);
	return (glm::degrees(pyl));
}

glm::mat4 SceneObject::generateModelMat()
{
	cout << glm::to_string(scale) << endl;
	glm::mat4 modelmat = glm::scale(getScale());
	modelmat = glm::toMat4(rotation) * modelmat;
	modelmat = glm::translate(getPosition()) * modelmat;
	if (isComponent) {
		modelmat = parentObject->generateModelMat() * modelmat;
	}
	return (modelmat);
}

void SceneObject::setIsComponent(bool isComp)
{
	isComponent = isComp;
}

void SceneObject::setParent(SceneObject* parent)
{
	parentObject = parent;
}
