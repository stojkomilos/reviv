#pragma once

#include"rv_pch.hpp"

#include"buffer.h"
#include"core/mat.h"

class Vao // VertexArrayObject
{
public:
    Vao() = default;
    ~Vao();

    void init();
    void load(unsigned int verticesSize, Vertex* pVertices, unsigned int indicesSize, unsigned int* pIndices);

    void bind() const;
    void unbind() const;

    GLuint id;

    void addVbo(const Vbo& vbo);
    void addEbo(const Ebo& ebo);
    std::vector<Vbo> vertexBuffers;
    std::vector<Ebo> elementBuffers;
};

void log(const Vao& vao);