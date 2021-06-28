#pragma once

#include<iostream>
#include<fstream> 
#include<vector>
#include"reviv/core/mat.h"
struct TripletOfInts {
	int x, y, z;
};
class ModelLoader {
public:
	ModelLoader();
	~ModelLoader();
	bool LoadModel(const char* path);
	char* pointer;
	size_t bufferSize;
	int nrTriangles, nrVertices;
};
