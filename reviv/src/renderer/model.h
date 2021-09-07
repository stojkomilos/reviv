#pragma once

#include"mesh.h"
#include"material.h"
#include"model_loader.h"

#include"stls/stable_vector.h"

struct ModelFlags
{
    bool isCullFaceOn = true;
};

class Model
{
public:
    Model() = default;
    Model(ModelLoader* pModelLoader, Material* pMaterial);
    Model(ModelLoader* pModelLoader, Shader* pShader);
    Model(ModelLoader* pModelLoader);

    ModelLoader* m_ModelLoader;
    ModelFlags flags;

    Material* addMaterial(Material* material);
    Material* addMaterialFromShader(Shader* shader);

    stls::StableVector<Mesh*> pMeshes;
    stls::StableVector<Material*> pMaterials;
};

void log(const Model& model);