#pragma once

#include<vector>

#include"buffer.h"
#include"core/entity.h"

class VertexArrayComponent : public SpecificComponent<VertexArrayComponent>
{
public:
	VertexArrayComponent();
	~VertexArrayComponent();
	void setUp();
	void const bind();
	void addVertexBuffer(VertexBufferComponent& vertexBuffer);
	std::vector<VertexBufferComponent> VertexBuffers;
	unsigned int ID;
	unsigned int nrOfTriangles;
};
