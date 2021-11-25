#include "EnvCube.h"

EnvCube::EnvCube(OGLCubeMapTexture* cm)
{
	cubeMap = cm;
	cube = new OGLVertexObject("res/models/cube.txt", true, 4);
	shader = new OGLTexturedShader("res/shaders/b.vert", "res/shaders/wcube.frag", 5, 4);
	shader->addUniform<OGLUniformMat4FV>("mvp");
	shader->addUniform<OGLUniform3FV>("camera_position");
	shader->addUniform<OGLUniformMat4FV>("world");
	shader->addUniform<OGLUniformFloat>("HDR");
	shader->addTexture(cubeMap);
	renderable = true;

	float hdrOn = 0.0f;
	shader->updateUniformData("HDR", &hdrOn);
}

//render4 at beginning of loop (for scene class)
void EnvCube::render(Camera* cam)
{
	glm::mat4 trmat = cam->getTransMat(false);
	glm::mat4 i = glm::identity<glm::mat4>();
	glm::vec3 campos = cam->getPosition();

	shader->updateUniformData("mvp", &trmat[0][0]);
	shader->updateUniformData("world", &i[0][0]);
	shader->updateUniformData("camera_position", &campos);

	cube->bind();
	shader->bindShaderProgram();
	
	glDepthMask(GL_FALSE);
	glDrawElements(GL_TRIANGLES, cube->getIndexCount(), GL_UNSIGNED_INT, (void*)0);
	glDepthMask(GL_TRUE);
}

void EnvCube::setCubeMap(OGLCubeMapTexture* cm)
{
	cubeMap = cm;
	// come back to this!!!!: delete shader;
	shader = new OGLTexturedShader("res/shaders/b.vert", "res/shaders/wcube.frag", 5, 4);
	shader->addUniform<OGLUniformMat4FV>("mvp");
	shader->addUniform<OGLUniform3FV>("camera_position");
	shader->addUniform<OGLUniformMat4FV>("world");
	shader->addUniform<OGLUniformFloat>("HDR");
	shader->addTexture(cubeMap);

	float hbool;
	hbool = (float)(HDRRendering ? 1 : 0);
	shader->updateUniformData("HDR", &hbool);
}

void EnvCube::setHDRRendering(bool hdr)
{
	HDRRendering = hdr;
	float hbool;
	hbool = (float)(hdr ? 1.0f : 0.0f);
	shader->updateUniformData("HDR", &hbool);
}
