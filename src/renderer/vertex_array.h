#pragma once

#include<vector>

#include"buffer.h"
#include"core/entity.h"

class Vao: public SpecificComponent<Vao>
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
