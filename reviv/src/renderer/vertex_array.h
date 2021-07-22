#pragma once

#include"rv_pch.hpp"

#include"buffer.h"
#include"core/mat.h"

class Vao // VertexArrayObject
{
public:
    Vao() : vertexBuffers(1), elementBuffers(1) {}
    ~Vao();

    void init();
    void load(unsigned int verticesSize, Vertex* pVertices, unsigned int indicesSize, unsigned int* pIndices);

    void bind() const;
    void unbind() const;

    GLuint id;

    void addVbo(const stls::StableVector<BufferElement>& inLayout);
    void addEbo();
    stls::StableVector<Vbo> vertexBuffers;
    stls::StableVector<Ebo> elementBuffers;
};