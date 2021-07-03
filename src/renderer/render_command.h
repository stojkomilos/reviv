#pragma once

#include"vertex_array.h"
#include"core/mat.h"

class RenderCommand
{
public:
	void drawArrays(const VaoComponent& object);
	void setClearColor(Vec4f color);
	void clear();
};
