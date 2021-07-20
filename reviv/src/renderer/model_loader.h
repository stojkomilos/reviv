#pragma once

#include"rv_pch.hpp"
#include"mesh.h"
#include"vertex_array.h"
#include"material.h"

#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>

class ModelLoader
{
public:
    ModelLoader() = default;

    void load(const std::string& filePath);

    std::string m_FilePath;
    void processNode(aiNode* node);
    void addMesh(aiMesh* loaderMesh, Mesh* pMesh);

    const aiScene* m_Scene;

    std::vector<Mesh> meshes;

    bool isLoaded = false;
};