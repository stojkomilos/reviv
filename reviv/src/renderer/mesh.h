#pragma once

#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>

#include"stls/stable_vector.h"
#include"core/mat.h"

struct Vertex
{
    Vec3f position;
    Vec3f normal;
    Vec3f TexCoords;
};

class Mesh
{
public:
    Mesh()
        : m_Vertices(10), indices(10) //TODO: ne moze da se zna pre ovoga
    {

    }
    void init(); // RV_ASSERT(sizeof(Vertex) == 32)
    stls::StableVector<Vertex>         m_Vertices;
    stls::StableVector<unsigned int>   indices;
    //stls::StableVector<Texture>        textures;

    unsigned int vao;
    unsigned int vbo;
    unsigned int ebo;
private:

};