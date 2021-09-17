#include"scene.h"
#include"components.h"

Entity* Scene::iCreateEntity(const std::string& entityName)
{
    entityList.emplaceBack(entityName);
    Entity* pEntity = &entityList[entityList.size()-1];
    pEntity->add<TransformComponent>();
    return pEntity;
}

Entity* Scene::iGetEntity(const std::string& entityName)
{
    for(auto itEntity = iGetEntityList()->begin(); itEntity != iGetEntityList()->end(); itEntity++)
    {
        if(itEntity->entityName == entityName)
        {
            return &(*itEntity);
        }
    }

#if RV_DEBUG // checks if there is more than one entity with the GIVEN name
    for(auto itEntity = iGetEntityList()->begin(); itEntity != iGetEntityList()->end(); itEntity++)
    {
        bool found = false;
        if(itEntity->entityName == entityName)
        {
            RV_ASSERT(found == false, "more than one entity with the given name");
            found = true;
        }
    }
#endif
    RV_ASSERT(false, "there is no entity with the given name");
    return nullptr;

}

void Scene::iLogEntity(const std::string& entityName)
{
    RV_ASSERT(false, "");
}

stls::StableVector<Entity>* Scene::iGetEntityList()
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

void Scene::debugProjectPosition(const Entity& entity)
{ 
    cout << "Projecting Position of: " << entity.entityName << "-----------" << endl;

    cout << "Position:" << endl;
    log(entity.get<TransformComponent>()->position);

    cout << "Transform" << endl;
    log(entity.get<TransformComponent>()->getTransform());

    cout << "afterTransform" << endl;
    auto afterTransform = multiply(entity.get<TransformComponent>()->getTransform(), Vec4f(0, 0, 0, 1));
    log(afterTransform);

    cout << "afterView" << endl;
    auto afterView = multiply(Scene::getCameraEntity()->get<CameraComponent>()->camera.viewMatrix, afterTransform);
    log(afterView);

    cout << "afterProjection" << endl;
    auto afterProjection = multiply(Scene::getCameraEntity()->get<CameraComponent>()->camera.projectionMatrix, afterView);
    log(afterProjection);

    cout << "afterNdc" << endl;
    auto afterNdc = afterProjection / afterProjection.a[3];
    log(afterNdc);
    cout << "End of projecting position------------------" << endl;
}

void Scene::iRemoveEntity(Entity* pEntity)
{
    RV_ASSERT(pEntity->valid == true, "entity already not valid");
    pEntity->valid = false;
}

bool Scene::iIsEntityValid(Entity* pEntity)
{
#ifdef RV_DEBUG
    bool sceneHas = false;
    for(auto itEntity = getEntityList()->begin(); itEntity != getEntityList()->end() ; itEntity++)
    {
        if(&(*itEntity) == pEntity)
        {
            sceneHas = true;
            break;
        }
    }
    RV_ASSERT(sceneHas == true, "entity does not belong to this scene");

#endif

    return pEntity->valid;
}