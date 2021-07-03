#include "buffer.h"

static unsigned int ShaderDataTypeSize(ShaderDataType type) {
	switch (type)
	{
		case ShaderDataType::Float:			return 4;
		case ShaderDataType::Float2:		return 4 * 2;
		case ShaderDataType::Float3:		return 4 * 3;
		case ShaderDataType::Float4:		return 4 * 4;
		case ShaderDataType::Mat3:			return 4 * 3 * 3;
		case ShaderDataType::Mat4:			return 4 * 4 * 4;
		case ShaderDataType::Int:			return 4;
		case ShaderDataType::Int2:			return 4 * 2;
		case ShaderDataType::Int3:			return 4 * 3;
		case ShaderDataType::Int4:			return 4 * 4;
		case ShaderDataType::Bool:			return 1;
	}
	std::cout << "ERROR: RENDERER UNKNOWN SHADER DATA TYPE\n";
	assert(false);
	return 0;
}

BufferElement::BufferElement(ShaderDataType type, std::string name, bool normalized)
	:name(name), type(type), size(ShaderDataTypeSize(type)), offset(0), normalized(normalized) {

}
BufferElement::BufferElement()
{

}
unsigned int BufferElement::getElementCount()
{
	switch (type) {
		case ShaderDataType::Float:			return 1;
		case ShaderDataType::Float2:		return 2;
		case ShaderDataType::Float3:		return 3;
		case ShaderDataType::Float4:		return 4;
		case ShaderDataType::Mat3:			return 3 * 3;
		case ShaderDataType::Mat4:			return 4 * 4;
		case ShaderDataType::Int:			return 1;
		case ShaderDataType::Int2:			return 2;
		case ShaderDataType::Int3:			return 3;
		case ShaderDataType::Int4:			return 4;
		case ShaderDataType::Bool:			return 1;
	}
	std::cout << "ERROR: RENDERER UNKNOWN SHADER DATA TYPE\n";
	assert(false);
	return 0;
}
unsigned int shaderDataTypeToOpenGLBaseType(ShaderDataType type) {
	switch (type)
	{
		case ShaderDataType::Float:			return GL_FLOAT;
		case ShaderDataType::Float2:		return GL_FLOAT;
		case ShaderDataType::Float3:		return GL_FLOAT;
		case ShaderDataType::Float4:		return GL_FLOAT;
		case ShaderDataType::Mat3:			return GL_FLOAT;
		case ShaderDataType::Mat4:			return GL_FLOAT;
		case ShaderDataType::Int:			return GL_INT;
		case ShaderDataType::Int2:			return GL_INT;
		case ShaderDataType::Int3:			return GL_INT;
		case ShaderDataType::Int4:			return GL_INT;
		case ShaderDataType::Bool:			return GL_BOOL;
	}
	std::cout << "ERROR: RENDERER UNKNOWN SHADER DATA TYPE\n";
	assert(false);
	return 0;
}



BufferLayout::BufferLayout(std::vector<BufferElement> elements)
	: elements(elements), stride(0)
{
	calculateOffsetsAndStride();
}
void BufferLayout::calculateOffsetsAndStride(){
	unsigned int offset = 0;
	for (int i = 0; i < elements.size(); i++)
	{
		elements[i].offset = offset;
		offset += elements[i].size;
		stride += elements[i].size;
	}
};

BufferLayout::BufferLayout() 
{
}


VertexBufferComponent::VertexBufferComponent()
{

}
VertexBufferComponent::~VertexBufferComponent()
{
	std::cout << "dodaj destruktor koji valja za VertexBuffer, obican, scope based nece radit, reference counting?, dynamicaly alocated?\n";
}

void VertexBufferComponent::bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VertexBufferComponent::setUp(void* vertices, unsigned int size, unsigned char typeOfDraw1)
{	

    const int INT_MAX = 2147483646; //quick change
	assert(GL_STATIC_DRAW < INT_MAX - 1 and GL_STATIC_DRAW > -INT_MAX + 1);
	assert(GL_DYNAMIC_DRAW < INT_MAX - 1 and GL_DYNAMIC_DRAW > -INT_MAX + 1);
	if (typeOfDraw1 == 0) {
		typeOfDraw = GL_STATIC_DRAW;
	}
	else if (typeOfDraw1 == 1) {
		typeOfDraw = GL_DYNAMIC_DRAW;
	}
	else {
		std::cout << "ERROR RENDERER: DRAW TYPE UNKOWN";
		assert(false);
	}
	count = size / layout.stride; ///prvo stavi layout vbo, pa onda setUp
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, typeOfDraw);
}
