#include"rv_pch.hpp"

#include"model.h"

void log(const Model& model)
{
    cout << "pMeshes.size(): " << model.pMeshes.size() << endl;

    for(auto it : model.pMeshes)
    {
        cout << "Mesh: " << endl;
        log(*it);
    }
}

Model::Model(ModelLoader* pModelLoader, Material* pMaterial)
{
    RV_ASSERT(pModelLoader->isLoaded, "");

    m_ModelLoader = pModelLoader;

    pMeshes.reserve(pModelLoader->meshes.size());
    pMaterials.reserve(pModelLoader->meshes.size());

    for(int i=0; i < pModelLoader->meshes.size(); i++)
    {
        pMeshes.pushBack(&pModelLoader->meshes[i]);
        pMaterials.pushBack(pMaterial);
    }
}