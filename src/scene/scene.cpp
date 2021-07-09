#include"scene.h"

Entity* Scene::iCreateEntity(const std::string& entityName)
{
    //cout << "CREATING ENTITY: " << entityName << endl;
    Entity da(entityName);
    //entityList.emplace(Entity(entityName));
    cout << "da_size: " << da.components.size() << " da_strSize: " << da.entityName.size() << endl;
    //entityList.emplace_back(da);
    entityList.push_back(da);
    return &entityList.back();
    //return &entityList[entityList.size()-1];
}

Entity* Scene::iFindEntity(const std::string& entityName)
{
 assert(false);
}

void Scene::iLogEntity(const std::string& entityName)
{
 assert(false);
}

std::vector<Entity>* Scene::iGetEntityList() //TODO: return const pointer?
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
void Scene::iSetCameraEntity(Entity* pNewCameraEntity)
{
    if(doesCameraEntityExist)
    {
        assert(false); 
    }  
    pCameraEntity = pNewCameraEntity;
    doesCameraEntityExist = true; 
}

Entity* Scene::iGetPlayerEntity()
{ 

//    assert(false);
    if(!doesPlayerEntityExist)
    {
        cout << "ERROOOOOOOOOOOOOOOOOOOOOOOR" << endl;
//        assert(false); 
    } 
    return pPlayerEntity; 
}
void Scene::iSetPlayerEntity(Entity* pNewPlayerEntity)
{
    if(doesPlayerEntityExist)
    {
        assert(false); 
    }  
    pPlayerEntity = pNewPlayerEntity;
    doesPlayerEntityExist = true; 
}
