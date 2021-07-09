#pragma once

#include<vector>

#include"buffer.h"

class Vao
{
public:
	unsigned int ID;
	unsigned int nrOfTriangles;


    Vbo vbo;
	std::vector<Vbo> VertexBuffers;
	Vao();
	~Vao();
	void addVertexBuffer(Vbo& vertexBuffer);
	void setUp();
	void bind() const;
};
