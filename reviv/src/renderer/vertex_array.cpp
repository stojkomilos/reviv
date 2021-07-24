#include "vertex_array.h"

using std::cin; using std::cout; using std::endl;

void Vao::init()
{
    glGenVertexArrays(1, &id);
}

Vao::~Vao()
{
    glDeleteVertexArrays(1, &id);
}

void Vao::bind() const
{
    glBindVertexArray(id);
}

void Vao::unbind() const
{
    glBindVertexArray(0);
}

void Vao::addVbo(const stls::StableVector<BufferElement>& inLayout)
{
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
    glBindVertexArray(id);
    elementBuffers.emplaceBack();
    Ebo* pEbo = &elementBuffers[elementBuffers.size() - 1];
    pEbo->pParentVao = this;
    pEbo->init();
}