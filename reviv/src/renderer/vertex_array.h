#pragma once

#include<vector>

#include"buffer.h"
#include"core/mat.h"
#include"renderer/model.h"

class Vao
{
public:
    Vao(const std::string& modelPath) { init(modelPath); }
    Vao(Model* pModel) : pModel(pModel) {}

    void init(const std::string& modelPath);
    void bind() const;

    Vbo vbo;
    Model* pModel;
    unsigned int ID;
    unsigned int nrOfTriangles;

    void addVertexBuffer(Vbo& vertexBuffer);
    //std::vector<Vbo> VertexBuffers;
};

void log(const Vao& vao);