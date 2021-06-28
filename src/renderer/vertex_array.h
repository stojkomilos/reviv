#pragma once

#include<vector>

#include"buffer.h"
#include"opengl_assert.h"

class VertexArray {
public:
	VertexArray();
	~VertexArray();
	void setUp();
	void const bind();
	void addVertexBuffer(VertexBuffer& vertexBuffer);
	std::vector<VertexBuffer> VertexBuffers;
	unsigned int ID;
	unsigned int nrOfTriangles;
};