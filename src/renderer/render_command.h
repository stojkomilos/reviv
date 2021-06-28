#pragma once

#include"vertex_array.h"
#include"core/mat.h"

class RenderCommand
{
public:
	void drawArrays(const VertexArray& object);
	void setClearColor(mat::vec4 color);
	void clear();
};
