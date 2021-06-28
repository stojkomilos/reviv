#pragma once

#include<glad/glad.h>

#include<vector>
#include<string>
#include<iostream>
#include<assert.h>

enum class ShaderDataType
{
	None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
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
	BufferLayout();
	BufferLayout(std::vector<BufferElement> elements);
	void calculateOffsetsAndStride();
	std::vector<BufferElement> elements;
	unsigned int stride;
};


class VertexBuffer 
{
public:
	VertexBuffer();
	~VertexBuffer();
	void bind();
	void unbind();
	void setUp(void* vertices, unsigned int size, unsigned char typeOfDraw1);
	unsigned int ID;
	int typeOfDraw;
	unsigned int count;
	BufferLayout layout;
};

unsigned int shaderDataTypeToOpenGLBaseType(ShaderDataType type);
