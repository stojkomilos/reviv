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

    cout << "pMaterials.size(): " << model.pMaterials.size() << endl;
    for(auto it : model.pMaterials)
    {
        cout << "Material: " << endl;
        log(*it);
    }
}

Model::Model(ModelLoader* pModelLoader, Material* pMaterial)
{
    RV_ASSERT(pModelLoader->isLoaded, "");

    m_ModelLoader = pModelLoader;

    pMeshes.reserve(pModelLoader->meshes.size());
    pMaterials.reserve(pModelLoader->meshes.size());

    for(unsigned int i=0; i < pModelLoader->meshes.size(); i++)
    {
        pMeshes.pushBack(&pModelLoader->meshes[i]);
        pMaterials.pushBack(pMaterial);
    }
}


Model::Model(ModelLoader* pModelLoader, Shader* pShader)
{
    RV_ASSERT(pModelLoader->isLoaded, "");

    m_ModelLoader = pModelLoader;

    pMeshes.reserve(pModelLoader->meshes.size());
    pMaterials.reserve(pModelLoader->meshes.size());

    Material* pMaterial = new Material(pShader);                             // TODO: no one really owns material then, no one will call the destructor or free the memory
    for(unsigned int i=0; i < pModelLoader->meshes.size(); i++)
    {
        pMeshes.pushBack(&pModelLoader->meshes[i]);
        pMaterials.pushBack(pMaterial);
    }
}

Model::Model(ModelLoader* pModelLoader)
{
    RV_ASSERT(pModelLoader->isLoaded, "");

    m_ModelLoader = pModelLoader;
    pMeshes.reserve(pModelLoader->meshes.size());
    pMaterials.reserve(5);

    for(unsigned int i=0; i < pModelLoader->meshes.size(); i++)
    {
        pMeshes.pushBack(&pModelLoader->meshes[i]);
    }
}

Material* Model::addMaterial(Material* pMaterial)
{
    RV_ASSERT(pMeshes.size() == 1 && pMaterials.size() == 0, ""); // can be implemented, but be careful

    pMaterials.pushBack(pMaterial);

    return pMaterial;
}

Material* Model::addMaterialFromShader(Shader* shader)
{
    RV_ASSERT(pMeshes.size() == 1 && pMaterials.size() == 0, ""); // can be implemented, but be careful

    Material* pMaterial = new Material(shader);
    pMaterials.pushBack(pMaterial);

    return pMaterial;
}