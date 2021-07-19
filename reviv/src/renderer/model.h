#pragma once

#include"mesh.h"

class Model
{
public:
    Model()
        : meshes(10) //TODO: ne moze da se zna ranije
    {

    }
    
    Model(const std::string& filePath);
    stls::StableVector<Mesh> meshes;
    std::string filePath;
    
};

struct TripletOfInts
{
	int x, y, z;
};



/*
#pragma once

#include"rv_pch.hpp"

#include"core/mat.h"
#include"scene/entity.h"
#include"core/time.h"

struct TripletOfInts
{
	int x, y, z;
};
class Model
{
public:
    Model() = default;
    ~Model();
    bool init(const char* path);
    char* pointer;
    size_t bufferSize;
    int nrTriangles, nrVertices;
};

void log(const Model& model);
*/
