#pragma once

#include<glad/glad.h>

#include"rv_pch.hpp"

enum class ShaderDataType
{
	None = 0, SdtFloat1, SdtFloat2, SdtFloat3, SdtFloat4, SdtMat3, SdtMat4, SdtInt1, SdtInt2, SdtInt3, SdtInt4, SdtBool
};

struct BufferElement 
{
	BufferElement(ShaderDataType type, std::string name, bool normalized);
	BufferElement();
	std::string name;
	ShaderDataType type;
	unsigned int size;
	unsigned int offset;
	bool normalized;
	unsigned int getElementCount();
	unsigned int shaderDataTypeToOpenGLBaseType(ShaderDataType type);
};

class BufferLayout
{
public:
	BufferLayout() = default;
	BufferLayout(std::vector<BufferElement> elements);
	void calculateOffsetsAndStride();
	std::vector<BufferElement> elements;
	unsigned int stride;
};


class Vbo
{
public:
	Vbo() = default;
	~Vbo();
	void bind();
	void unbind();
	void init(void* vertices, unsigned int size, unsigned char typeOfDraw1);
	unsigned int ID;
	int typeOfDraw;
	unsigned int count;
	BufferLayout layout;
};

unsigned int shaderDataTypeToOpenGLBaseType(ShaderDataType type);
