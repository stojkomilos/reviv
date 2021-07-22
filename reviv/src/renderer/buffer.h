#pragma once

#include<glad/glad.h>

#include"rv_pch.hpp"
#include"core/mat.h"

struct Vertex
{
    Vec3f position;
    Vec3f normal;
    Vec2f texCoords;
};

enum class ShaderDataType
{
	None = 0, SdtFloat1, SdtFloat2, SdtFloat3, SdtFloat4, SdtMat3, SdtMat4, SdtInt1, SdtInt2, SdtInt3, SdtInt4, SdtBool
};

struct BufferElement 
{
    BufferElement() = default;
    BufferElement(ShaderDataType type, std::string name, bool normalized);

    unsigned int getElementCount() const;
    unsigned int shaderDataTypeToOpenGLBaseType(ShaderDataType type);

    std::string name;
    ShaderDataType type;
    unsigned int size;
    char* offset;
    bool normalized;
};

class BufferLayout
{
public:
    BufferLayout() = default;
    BufferLayout(stls::StableVector<BufferElement> inLayout);
    void init();
    stls::StableVector<BufferElement> elements;
    int stride = -1;
};

class Vao;

class GBufferObject
{
public:
    GBufferObject() = default; //TODO: delete
    GBufferObject(const GBufferObject&) = default; //TODO: delete
    virtual ~GBufferObject();

    GBufferObject& operator=(const GBufferObject&) = default; //TODO: delete

    void bind() const;
    void unbind() const;
    void init();
    void load(void* pData, unsigned int size);

    GLenum bufferType;
    GLuint id;

    Vao* pParentVao = nullptr;

protected:
    GBufferObject(int bufferType)
        : bufferType(bufferType) { RV_ASSERT(bufferType == GL_ARRAY_BUFFER or bufferType == GL_ELEMENT_ARRAY_BUFFER, "buffer type not recognized"); }
};

class Vbo : public GBufferObject // VertexBufferObject
{
public:
    Vbo() : GBufferObject(GL_ARRAY_BUFFER) {}
    BufferLayout layout;
};

class Ebo : public GBufferObject // ElementBufferObject
{
public:
    Ebo() : GBufferObject(GL_ELEMENT_ARRAY_BUFFER) {}
};

unsigned int shaderDataTypeToOpenGLBaseType(ShaderDataType type);