#include "OGLVertexObject.h"

OGLVertexObject::OGLVertexObject(int numVertices, int maxAttributes)
{
	maxAttribs = maxAttributes;
	vertexCount = numVertices;
	glGenVertexArrays(1, &vao);
	
	bufferids = new unsigned int[maxAttributes];
	bufferdata = new void*[maxAttributes];
	numBuffers = 0;
	ibo = 0;
}

//enabling normal mapping on mesh will cause it to create tangent vectors for each vertex that can be used in the shader
OGLVertexObject::OGLVertexObject(string fname, bool normalMapping, int maxAttributes)
{
	int minAttribs = normalMapping ? 4: 3;
	if (maxAttributes < minAttribs) {
		maxAttributes = minAttribs;
	}
	maxAttribs = maxAttributes;
	
	glGenVertexArrays(1, &vao);
	bufferids = new unsigned int[maxAttributes];
	bufferdata = new void* [maxAttributes];
	numBuffers = 0;
	ibo = 0;

	if (normalMapping) {
		auto vAttribs = loadModelFromOBJ(fname, &vertexCount, 1, true);
		addAttribute(0, 3, vAttribs[0]);
		addAttribute(1, 3, vAttribs[1]);
		addAttribute(2, 2, vAttribs[2]);
		addAttribute(3, 3, vAttribs[3]);
		addIndexing((unsigned int*)(vAttribs[4]), vertexCount);
	}
	else {
		auto vAttribs = loadModelFromOBJ(fname, &vertexCount);
		addAttribute(0, 3, vAttribs[0]);
		addAttribute(1, 3, vAttribs[1]);
		addAttribute(2, 2, vAttribs[2]);
		addIndexing((unsigned int*)(vAttribs[3]), vertexCount);
	}
}

OGLVertexObject::~OGLVertexObject()
{
	glDeleteVertexArrays(1, &vao);
	delete[] bufferids;
	cout << numBuffers << endl;
	for (int i = 0; i < numBuffers; i++) {
		//delete bufferdata[i];
	}
	delete[] bufferdata;
}

//add changing attributes
void OGLVertexObject::addAttribute(int attribNum, int numItems, float* vbuffer)
{
	if (attribNum >= maxAttribs) {
		applicationErrorCallback("attribute number greater than specified maximum number of attributes");
	}
	glBindVertexArray(vao);
	glGenBuffers(1, bufferids + numBuffers);
	glBindBuffer(GL_ARRAY_BUFFER, bufferids[numBuffers]);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * numItems * sizeof(float),  vbuffer, GL_STATIC_DRAW);

	cout << attribNum << endl;
	glEnableVertexAttribArray(attribNum);
	glVertexAttribPointer(attribNum, numItems, GL_FLOAT, GL_FALSE, 0, 0);
	glBindVertexArray(0);

	bufferdata[numBuffers] = (void*)vbuffer;
	numBuffers++;
}

void OGLVertexObject::addAttribute(int* attribNums, int* numItems, float* vbuffer)
{

}

void OGLVertexObject::addIndexing(unsigned int* ibuffer, int numIndices)
{
	this->numIndices = numIndices;
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

int OGLVertexObject::getVertexCount()
{
	return vertexCount;
}

int OGLVertexObject::getIndexCount()
{
	return numIndices;
}

void* OGLVertexObject::getBufferData(int attribNum)
{
	return (bufferdata[attribNum]);
}
