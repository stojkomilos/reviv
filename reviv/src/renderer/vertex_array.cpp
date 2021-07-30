#include "vertex_array.h"

using std::cin; using std::cout; using std::endl;

void Vao::init()
{
    RV_ASSERT(isInited == false, "");
    isInited = true;
    glGenVertexArrays(1, &id);
}

Vao::~Vao()
{
    RV_ASSERT(isInited == true, "");
    glDeleteVertexArrays(1, &id);
}

void Vao::bind() const
{
    RV_ASSERT(isInited == true, "");
    glBindVertexArray(id);
}

void Vao::unbind() const
{
    RV_ASSERT(isInited == true, "");
    glBindVertexArray(0);
}

void Vao::addVbo(const stls::StableVector<BufferElement>& inLayout)
{
    RV_ASSERT(isInited == true, "");
    glBindVertexArray(id);

    vertexBuffers.emplaceBack();
    Vbo* pVbo = &vertexBuffers[vertexBuffers.size()-1];
    pVbo->layout.elements = inLayout;
    pVbo->layout.init();
    pVbo->pParentVao = this;

    pVbo->init();
    pVbo->bind();

    RV_ASSERT(pVbo->layout.elements.size() != 0, "invalid layout")

    for (unsigned int i = 0; i < pVbo->layout.elements.size(); i++)
    {
        glVertexAttribPointer(i,
            pVbo->layout.elements[i].getElementCount(),
            shaderDataTypeToOpenGLBaseType(pVbo->layout.elements[i].type),
            pVbo->layout.elements[i].normalized,
            pVbo->layout.stride,
            pVbo->layout.elements[i].offset);

        glEnableVertexAttribArray(i);
    }
}

void Vao::addEbo()
{
    RV_ASSERT(isInited == true, "");
    glBindVertexArray(id);
    elementBuffers.emplaceBack();
    Ebo* pEbo = &elementBuffers[elementBuffers.size() - 1];
    pEbo->pParentVao = this;
    pEbo->init();
}