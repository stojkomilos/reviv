#include"scene.h"

Entity* Scene::iCreateEntity(const std::string& entityName)
{
    entityList.emplaceBack(entityName);
    return &entityList[entityList.size()-1];
}

Entity* Scene::iFindEntity(const std::string& entityName)
{
    RV_ASSERT(false, "");
    return nullptr;
}

void Scene::iLogEntity(const std::string& entityName)
{
    RV_ASSERT(false, "");
}

stls::StableVector<Entity>* Scene::iGetEntityList() //TODO: return const pointer?
{
    return &entityList;
}


Entity* Scene::iGetCameraEntity()
{ 
    RV_ASSERT(doesCameraEntityExist, "trying to get camera, but it dosn't exist");
    return pCameraEntity; 
}
Entity* Scene::iSetCameraEntity(Entity* pNewCameraEntity)
{
    RV_ASSERT(!doesCameraEntityExist, "Setting camra, but it alreadye exists");
    pCameraEntity = pNewCameraEntity;
    doesCameraEntityExist = true; 

    return pCameraEntity;
}

Entity* Scene::iGetPlayerEntity()
{ 
    RV_ASSERT(doesPlayerEntityExist, "Trying to get player, but it does not exist");
    return pPlayerEntity; 
}

Entity* Scene::iSetPlayerEntity(Entity* pNewPlayerEntity)
{
    RV_ASSERT(!doesPlayerEntityExist, "trying to set player, but it already exists")
    pPlayerEntity = pNewPlayerEntity;
    doesPlayerEntityExist = true; 

    return pPlayerEntity;
}
