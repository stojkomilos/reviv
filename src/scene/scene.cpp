#include"scene.h"

Entity* Scene::iCreateEntity(const std::string& entityName)
{
    //Entity da(entityName);
    //cout << "da_size: " << da.components.size() << " da_strSize: " << da.entityName.size() << endl;
    cout << "asdf";
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
        cout << "ERROOOOOOOOOOOOOOOOOOOOOOOR" << endl; //TODO ocisti ove komentare, i nepotrban ispis
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
