#pragma once

#include"mesh.h"
#include"material.h"
#include"model_loader.h"

#include"stls/stable_vector.h"

class Model
{
public:
    Model() = delete;
    Model(ModelLoader* pModelLoader, Material* pMaterial);

    ModelLoader* m_ModelLoader;

    stls::StableVector<Mesh*> pMeshes;
    stls::StableVector<Material*> pMaterials;
};

void log(const Model& model);