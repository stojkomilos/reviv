#pragma once

#include<vector>

#include"buffer.h"
#include"core/entity.h"

class VaoComponent : public SpecificComponent<VaoComponent>
{
public:
	VaoComponent();
	~VaoComponent();
	void setUp();
	void const bind();
	void addVertexBuffer(Vbo& vertexBuffer);
	std::vector<Vbo> VertexBuffers;
	unsigned int ID;
	unsigned int nrOfTriangles;
};
