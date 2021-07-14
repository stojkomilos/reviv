#include"scene.h"

Entity* Scene::iCreateEntity(const std::string& entityName)
{
    entityList.emplaceBack(entityName);
    return &entityList[entityList.size()-1];
}

Entity* Scene::iFindEntity(const std::string& entityName)
{
 assert(false);
}

void Scene::iLogEntity(const std::string& entityName)
{
    assert(false);
}

stls::StableVector<Entity>* Scene::iGetEntityList() //TODO: return const pointer?
{
    return &entityList;
}


Entity* Scene::iGetCameraEntity()
{ 
    if(!doesCameraEntityExist)
    {
        assert(false); 
    } 
    return pCameraEntity; 
}
Entity* Scene::iSetCameraEntity(Entity* pNewCameraEntity)
{
    if(doesCameraEntityExist)
    {
        assert(false); 
    }  
    pCameraEntity = pNewCameraEntity;
    doesCameraEntityExist = true; 

    return pCameraEntity;
}

Entity* Scene::iGetPlayerEntity()
{ 
    if(!doesPlayerEntityExist)
    {
        assert(false); 
    } 
    return pPlayerEntity; 
}

Entity* Scene::iSetPlayerEntity(Entity* pNewPlayerEntity)
{
    if(doesPlayerEntityExist)
    {
        assert(false); 
    }  
    pPlayerEntity = pNewPlayerEntity;
    doesPlayerEntityExist = true; 

    return pPlayerEntity;
}
