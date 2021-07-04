#pragma once

#include"vertex_array.h"
#include"core/mat.h"

class RenderCommand
{
public:
	void drawArrays(const Vao& object);
	void setClearColor(Vec4f color);
	void clear();
};
