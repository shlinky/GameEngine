#include "OGLVertexObject.h"

OGLVertexObject::OGLVertexObject(int numVertices, int maxAttributes)
{
	vertexCount = numVertices;
	glGenVertexArrays(1, &vao);
	
	bufferids = new unsigned int[maxAttributes];
	numBuffers = 0;
	ibo = 0;
}

OGLVertexObject::~OGLVertexObject()
{
	glDeleteVertexArrays(1, &vao);
	delete bufferids;
}

//add changing attributes
void OGLVertexObject::addAttribute(int attribNum, int numItems, float* vbuffer)
{
	glBindVertexArray(vao);
	glGenBuffers(1, bufferids + numBuffers);
	glBindBuffer(GL_ARRAY_BUFFER, bufferids[numBuffers]);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * numItems * sizeof(float),  vbuffer, GL_STATIC_DRAW);

	glEnableVertexAttribArray(attribNum);
	glVertexAttribPointer(attribNum, numItems, GL_FLOAT, GL_FALSE, 0, 0);
	glBindVertexArray(0);

	numBuffers++;
}

void OGLVertexObject::addAttribute(int* attribNums, int* numItems, float* vbuffer)
{

}

void OGLVertexObject::addIndexing(unsigned int* ibuffer, int numIndices)
{
	glBindVertexArray(vao);
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(float), ibuffer, GL_STATIC_DRAW);
	glBindVertexArray(0);
}

void OGLVertexObject::bind()
{
	glBindVertexArray(vao);
}
