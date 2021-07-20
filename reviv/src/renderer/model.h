#pragma once

#include"mesh.h"
#include"material.h"
#include"model_loader.h"

class Model
{
public:
    Model() = delete;
    Model(ModelLoader* pModelLoader, Material* pMaterial);

    ModelLoader* m_ModelLoader;

    std::vector<Mesh*> pMeshes;
    std::vector<Material*> pMaterials;
};

void log(const Model& model);