#pragma once

#include<iostream>
#include<fstream> 
#include<vector>
#include"core/mat.h"
#include"scene/entity.h"

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