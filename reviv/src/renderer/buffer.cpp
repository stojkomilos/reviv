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

unsigned int BufferElement::getElementCount() const
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

void BufferLayout::init()
{
    RV_ASSERT(elements.size() != 0, "");
    unsigned int offset = 0;
    stride = 0;
    for (int i = 0; i < elements.size(); i++)
    {
        elements[i].offset = offset;
        offset += elements[i].size;
        stride += elements[i].size;
    }
};

BufferLayout::BufferLayout(std::vector<BufferElement> inLayout)
{
    for(int i=0; i < inLayout.size(); i++)
    {
        elements.push_back(inLayout[i]);
    }
}


GBufferObject::~GBufferObject()
{
    glDeleteBuffers(1, &id);
}

void GBufferObject::init()
{
    glGenBuffers(1, &id);
}

void GBufferObject::bind() const
{
    cout << "bufferType: " << bufferType << endl;
    glBindBuffer(bufferType, id);
}

void GBufferObject::unbind() const
{
    glBindBuffer(bufferType, 0);
}


void GBufferObject::load(void* pData, unsigned int size)
{
    glBindBuffer(bufferType, id);
    glBufferData(bufferType, size, pData, GL_STATIC_DRAW);
}