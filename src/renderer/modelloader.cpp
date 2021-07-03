#include"modelloader.h"

#define _CRT_SECURE_NO_WARNINGS

// Novo TODO: reformisati ModelLoader klasu, tako da nema svaka instanca .Load funciju. Mozda nek to bude neka globna funkcija, mozda metoda Render_manager-a, mozda samo static metoda kod ModelLoader-a? idk

#include<ctime>
#include<vector>
#include<cassert>

ModelLoader::ModelLoader() {
	nrTriangles = nrVertices = 0;
	
}
ModelLoader::~ModelLoader() {
	delete[] pointer;
}
bool ModelLoader::LoadModel(const char* path) { 
	std::cout << "Start reading of model " << path << std::endl;
	clock_t clock = std::clock();

	std::vector<Vec3f> vVertices;
	std::vector<Vec2f> vTextures;
	std::vector<Vec3f> vNormals;
	std::vector<TripletOfInts> vVertexIndices;
	std::vector<TripletOfInts> vTextureIndices;
	std::vector<TripletOfInts> vNormalIndices;

	FILE* file = fopen(path, "r");
	if (file == NULL)
		std::cout << "ERROR READING MODEL, CAN'T OPEN FILE: " << path << std::endl;


	char c;


	while (1) {
		Vec3f vertex;
		Vec2f textureVertex;
		char antiWarningChar = fscanf(file, "%c", &c);
		if (antiWarningChar == EOF) {
			break;
		}
		if (c == 'v') {
			bool isTexture = false;
			bool isNormal = false;
			antiWarningChar = fscanf(file, "%c", &c);
			if (c == 't') {
				antiWarningChar = fscanf(file, "%c", &c);
				if (c != ' ') {
					assert(false);
					continue;
				}
				isTexture = true;
			}
			if (c == 'n') {
				antiWarningChar = fscanf(file, "%c", &c);
				if (c != ' ') {
					assert(false);
					continue;
				}
				isNormal = true;
			}
			if (isTexture) {
				antiWarningChar = fscanf(file, "%f %f", &textureVertex.x, &textureVertex.y);
			}
			else {
				antiWarningChar = fscanf(file, "%f %f %f", &vertex.x, &vertex.y, &vertex.z);
			}
			antiWarningChar = fscanf(file, "%c", &c);
			if (isTexture) {
				vTextures.push_back(textureVertex);
			}
			else if (isNormal) {
				vNormals.push_back(vertex);
			}
			else { vVertices.push_back(vertex); }
			if (antiWarningChar == EOF) {
				break;
			}
		}
		if (c == 'f') {
			antiWarningChar = fscanf(file, "%c", &c);
			if (c == ' ') {
				int currentFace = 0; ///curentFace je da kazem 0 indexed
				TripletOfInts vertexIndicesTemp, textureIndicesTemp, normalIndicesTemp;
				int vertexIndex, textureIndex, normalIndex;
				bool doubleBreak = false;
				while (c != '\n') {
					char c1, c2;

					antiWarningChar = fscanf(file, "%d%c%d%c%d", &vertexIndex, &c1, &textureIndex, &c2, &normalIndex);
					antiWarningChar = fscanf(file, "%c", &c);
					//std::cout << "fuck " << vertexIndex << c1 << textureIndex << c2 << normalIndex << std::endl;

					vertexIndex--;
					textureIndex--;
					normalIndex--;

					if (currentFace == 3) {
						vertexIndicesTemp.y = vertexIndicesTemp.z;
						textureIndicesTemp.y = textureIndicesTemp.z;
						normalIndicesTemp.y = normalIndicesTemp.z;
						currentFace = 2;
					}
					assert(currentFace <= 2);
					*((int*)(&vertexIndicesTemp) + currentFace) = vertexIndex;
					*((int*)(&textureIndicesTemp) + currentFace) = textureIndex;
					*((int*)(&normalIndicesTemp) + currentFace) = normalIndex; // ne dozvoli da currentFace bude preko 2 zbog leaka


					if (currentFace == 2) {

						//std::cout << "pushingGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG:\n";
						//std::cout << vertexIndicesTemp.x << " " << vertexIndicesTemp.y << " " << vertexIndicesTemp.z << std::endl;
						//std::cout << textureIndicesTemp.x << " " << textureIndicesTemp.y << " " << textureIndicesTemp.z << std::endl;
						//std::cout << normalIndicesTemp.x << " " << normalIndicesTemp.y << " " << normalIndicesTemp.z << std::endl;
						vVertexIndices.push_back(vertexIndicesTemp);
						vTextureIndices.push_back(textureIndicesTemp);
						vNormalIndices.push_back(normalIndicesTemp);
					}
					currentFace++;

					if (antiWarningChar == EOF) {
						doubleBreak = true;
						break;
					}
				}
				if (doubleBreak) {
					break;
				}
			}
			else {
				assert(false);
				continue;
			}
		}
	}

	fclose(file);


	bufferSize = vVertexIndices.size() * 3 * (2 * sizeof(Vec3f) + sizeof(Vec2f));   
	pointer = new char[bufferSize];
	for (int i = 0; i < vVertexIndices.size(); i ++){ ///ovo ubacivanje u heap izgleda cudno jer je polu prilagodjeno za koriscenje element array objecta, al trenutno ne koristim
		for (int j = 0; j < 3; j++) {
			char* address = pointer + (sizeof(Vec3f) * 2 + sizeof(Vec2f)) * (i * 3 + j);


			*((Vec3f*)(address))                                         = vVertices[*((int*)((char*)&vVertexIndices[i].x  + j * sizeof(int)))];
			*((Vec2f*)(address + sizeof(Vec3f)))                     = vTextures[*((int*)((char*)&vTextureIndices[i].x + j * sizeof(int)))];
			*((Vec3f*)(address + sizeof(Vec3f) + sizeof(Vec2f))) = vNormals [*((int*)((char*)&vNormalIndices[i].x  + j * sizeof(int)))];
		}
	}


	std::cout << "bufferSize = " << bufferSize << std::endl;
	std::cout << "vVertices = " << vVertices.size() << " vTextures = " << vTextures.size() << " vNormals = " << vNormals.size() << std::endl;
	std::cout << "vertIndices = " << vVertexIndices.size() << " textIndices = " << vTextureIndices.size() << " vNormalIndices = " << vNormalIndices.size() << std::endl;


	nrVertices = vVertices.size();
	nrTriangles = vVertexIndices.size(); ///vertex buffer layout, cherno ima dobar video, meni su svi trenutno zajedno a ne po jednom vertexu sto mozda ne valja za cash ili tako nesto

	std::cout << "End of reading model " << path << std::endl << "#triangles " << nrTriangles << " #vertices " << nrVertices << std::endl << "Time: " << (std::clock() - clock) / (CLOCKS_PER_SEC * 1.0f) << " Data size " << nrTriangles * sizeof(Vec3f) / 1024.f << "KB\n";

	vVertices.clear();
	
	return true;
}
