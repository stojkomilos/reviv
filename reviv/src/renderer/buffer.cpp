#include "buffer.h"

unsigned int ShaderDataTypeSize(ShaderDataType type)
{
    switch (type)
    {
        case ShaderDataType::SdtFloat1:			return 4;
        case ShaderDataType::SdtFloat2:		return 4 * 2;
        case ShaderDataType::SdtFloat3:		return 4 * 3;
        case ShaderDataType::SdtFloat4:		return 4 * 4;
        case ShaderDataType::SdtMat3:			return 4 * 3 * 3;
        case ShaderDataType::SdtMat4:			return 4 * 4 * 4;
        case ShaderDataType::SdtInt1:			return 4;
        case ShaderDataType::SdtInt2:			return 4 * 2;
        case ShaderDataType::SdtInt3:			return 4 * 3;
        case ShaderDataType::SdtInt4:			return 4 * 4;
        case ShaderDataType::SdtBool:			return 1;
        default: RV_ASSERT(false, "ERROR: RENDERER UNKNOWN SHADER DATA TYPE"); break;
    }
    return 0;
}

BufferElement::BufferElement(ShaderDataType type, std::string name, bool normalized)
    :name(name), type(type), size(ShaderDataTypeSize(type)), offset(0), normalized(normalized)
{

}

BufferElement::BufferElement()
{

}

unsigned int BufferElement::getElementCount()
{
    switch (type) {
        case ShaderDataType::SdtFloat1:			return 1;
        case ShaderDataType::SdtFloat2:		return 2;
        case ShaderDataType::SdtFloat3:		return 3;
        case ShaderDataType::SdtFloat4:		return 4;
        case ShaderDataType::SdtMat3:			return 3 * 3;
        case ShaderDataType::SdtMat4:			return 4 * 4;
        case ShaderDataType::SdtInt1:			return 1;
        case ShaderDataType::SdtInt2:			return 2;
        case ShaderDataType::SdtInt3:			return 3;
        case ShaderDataType::SdtInt4:			return 4;
        case ShaderDataType::SdtBool:			return 1;
    }
    RV_ASSERT(false, "ERROR: RENDERER UNKNOWN SHADER DATA TYPE");
    return 0;
}

unsigned int shaderDataTypeToOpenGLBaseType(ShaderDataType type)
{
    switch (type)
    {
        case ShaderDataType::SdtFloat1:	    return GL_FLOAT;
        case ShaderDataType::SdtFloat2:     return GL_FLOAT;
        case ShaderDataType::SdtFloat3:	    return GL_FLOAT;
        case ShaderDataType::SdtFloat4:	    return GL_FLOAT;
        case ShaderDataType::SdtMat3:       return GL_FLOAT;
        case ShaderDataType::SdtMat4:       return GL_FLOAT;
        case ShaderDataType::SdtInt1:       return GL_INT;
        case ShaderDataType::SdtInt2:       return GL_INT;
        case ShaderDataType::SdtInt3:       return GL_INT;
        case ShaderDataType::SdtInt4:       return GL_INT;
        case ShaderDataType::SdtBool:       return GL_BOOL;
    }
    RV_ASSERT(false, "ERROR: RENDERER UNKNOWN SHADER DATA TYPE");
    return 0;
}

BufferLayout::BufferLayout(std::vector<BufferElement> elements)
	: elements(elements), stride(0)
{
	calculateOffsetsAndStride();
}

void BufferLayout::calculateOffsetsAndStride()
{
	unsigned int offset = 0;
	for (int i = 0; i < elements.size(); i++)
	{
		elements[i].offset = offset;
		offset += elements[i].size;
		stride += elements[i].size;
	}
};

Vbo::~Vbo()
{
	std::cout << "dodaj destruktor koji valja za VertexBuffer, obican, scope based nece radit, reference counting?, dynamicaly alocated?\n";
}

void Vbo::bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void Vbo::init(void* vertices, unsigned int size, unsigned char typeOfDraw1)
{	

    const int INT_MAX = 2147483646; //quick change
    RV_ASSERT(GL_STATIC_DRAW < INT_MAX - 1 and GL_STATIC_DRAW > -INT_MAX + 1, "");
    RV_ASSERT(GL_DYNAMIC_DRAW < INT_MAX - 1 and GL_DYNAMIC_DRAW > -INT_MAX + 1, "");
    if (typeOfDraw1 == 0) {
        typeOfDraw = GL_STATIC_DRAW;
    }
    else if (typeOfDraw1 == 1) {
        typeOfDraw = GL_DYNAMIC_DRAW;
    }
    else {
        RV_ASSERT(false, "ERROR RENDERER: Draw type unknown");
    }
    count = size / layout.stride; ///prvo stavi layout vbo, pa onda setUp
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ARRAY_BUFFER, ID);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, typeOfDraw);
}