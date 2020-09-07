#pragma once
#include <GL/glew.h>
#include <iostream>
using namespace std;

class OGLVertexObject
{
public:
	OGLVertexObject(int numVertices, int maxAttributes);
	~OGLVertexObject();
	void addAttribute(int attribNum, int numItems, float* vbuffer);
	void addAttribute(int* attribNums, int* numItems, float* vbuffer);
	void addIndexing(unsigned int* ibuffer, int numIndices);
	void bind();
private:
	unsigned int vao;
	unsigned int* bufferids;
	unsigned int ibo;
	int vertexCount;
	int numBuffers;
};

