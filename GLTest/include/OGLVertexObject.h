#pragma once
#include <GL/glew.h>
#include <iostream>
#include <ModelLoader.h>
using namespace std;

class OGLVertexObject
{
public:
	OGLVertexObject(int numVertices, int maxAttributes = 3);
	OGLVertexObject(string fname, bool normalMapping = false, int maxAttributes = 3);
	~OGLVertexObject();
	void addAttribute(int attribNum, int numItems, float* vbuffer);
	void addAttribute(int* attribNums, int* numItems, float* vbuffer);
	void addIndexing(unsigned int* ibuffer, int numIndices);
	void bind();
	int getVertexCount();
	int getIndexCount();
	void* getBufferData(int attribNum);
private:
	unsigned int vao;
	unsigned int* bufferids;
	void** bufferdata;
	unsigned int ibo;
	int vertexCount;
	int numBuffers;
	int maxAttribs;
	int numIndices;
};

