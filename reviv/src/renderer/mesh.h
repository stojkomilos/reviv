#pragma once

#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>

#include"stls/stable_vector.h"
#include"core/mat.h"
#include"renderer/vertex_array.h"

void log(const Vertex& vertex);

class Mesh
{
public:
    Mesh() = default;
    ~Mesh() = default;
    Mesh(const Mesh&) = delete;

    Mesh(Mesh&&) = default;

    Mesh& operator=(const Mesh&) = delete;

    stls::StableVector<Vertex> m_Vertices;
    stls::StableVector<unsigned int> m_Indices;
    Vao vao;
};

void log(const Mesh& mesh);