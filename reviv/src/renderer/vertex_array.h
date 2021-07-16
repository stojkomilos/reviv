#pragma once

#include<vector>

#include"buffer.h"
#include"core/mat.h"
#include"renderer/model.h"

class Vao
{
public:
    //Vao(const std::string& modelPath) { init(modelPath); }
    Vao() = default;
    Vao(const std::string& modelPath);
    ~Vao();

    void bind() const;

    Vbo vbo;
    Model* pModel;
    unsigned int ID;
    unsigned int nrOfTriangles;

    void addVertexBuffer(Vbo& vertexBuffer);
};

void log(const Vao& vao);