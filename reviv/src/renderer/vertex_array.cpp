#include "vertex_array.h"

using std::cin; using std::cout; using std::endl;

void log(const Vao& vao)
{
    RV_ASSERT(false, "not defined");
}

void Vao::init()
{
    glGenVertexArrays(1, &id);
}


Vao::~Vao()
{
    glDeleteBuffers(1, &id);
}

void Vao::bind() const
{
    glBindVertexArray(id);
}

void Vao::unbind() const
{
    glBindVertexArray(0);
}


void Vao::addVbo(const Vbo& vbo)
{
    glBindVertexArray(id);

    vertexBuffers.push_back(vbo);

    Vbo* pVbo = &vertexBuffers[vertexBuffers.size()-1];
    pVbo->bind();

    RV_ASSERT(pVbo->layout.elements.size() != 0, "layout not added to the vbo")

    for (int i = 0; i < pVbo->layout.elements.size(); i++)
    {
        //void* a = (void*)pVbo->layout.elements[i].offset;
        glVertexAttribPointer(i,
            pVbo->layout.elements[i].getElementCount(),
            shaderDataTypeToOpenGLBaseType(pVbo->layout.elements[i].type),
            pVbo->layout.elements[i].normalized,
            pVbo->layout.stride,
            (void*)pVbo->layout.elements[i].offset);

        glEnableVertexAttribArray(i);
    }
}

void Vao::addEbo(const Ebo& ebo)
{
    glBindVertexArray(id);
    elementBuffers.push_back(ebo);
    elementBuffers[elementBuffers.size()-1].bind();
}