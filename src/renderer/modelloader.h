#pragma once

#include<iostream>
#include<fstream> 
#include<vector>
#include"core/mat.h"
#include"core/entity.h"

struct TripletOfInts
{
	int x, y, z;
};
class ModelLoader : public SpecificComponent<ModelLoader>
{
public:
	ModelLoader();
	~ModelLoader();
	bool LoadModel(const char* path);
	char* pointer;
	size_t bufferSize;
	int nrTriangles, nrVertices;
};
