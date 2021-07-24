#pragma once

#include"mesh.h"
#include"material.h"
#include"model_loader.h"

#include"stls/stable_vector.h"

class Model
{
public:
    Model() = default;
    Model(ModelLoader* pModelLoader, Material* pMaterial);
    Model(ModelLoader* pModelLoader);

    ModelLoader* m_ModelLoader;

    Material* addMaterial(const Shader& shader);

    stls::StableVector<Mesh*> pMeshes;
    stls::StableVector<Material*> pMaterials;
};

void log(const Model& model);