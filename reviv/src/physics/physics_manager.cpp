#include"physics_manager.h"
#include"renderer/camera.h"

#include"scene/scene.h"
#include"scene/components.h"

//extern Entity* stanic;

void PhysicsManager::iOnUpdate()
{

    alignPositionAndRotation(*Scene::getPlayerEntity(), Scene::getCameraEntity());

    updateTransforms();
}

void PhysicsManager::iInit()
{

}

void PhysicsManager::updateTransforms() // updates the transforms of all the entities according to the positions and stuff
{
    for(auto itEntity = Scene::getEntityList()->begin(); itEntity != Scene::getEntityList()->end(); itEntity++)
    {
        if(itEntity->has<TransformComponent>() and itEntity->has<PositionComponent>())
        {
            //cout << "Updating tranform for entity: " << itEntity->entityName << endl;
            itEntity->get<TransformComponent>()->transform = translate(identity, itEntity->get<PositionComponent>()->position);
        }
    }
}

void PhysicsManager::alignPositionAndRotation(const Entity& parent, Entity* child)
{

    assert(parent.has<PositionComponent>() // ERROR: Required components are not present in entity\n
        and parent.has<RotationComponent>()
        and child->has<PositionComponent>()
        and child->has<RotationComponent>()); 

    auto* parentPosition = &parent.get<PositionComponent>()->position;
    auto* parentRotation = &parent.get<RotationComponent>()->rotation;
    auto* pChildPosition = &child->get<PositionComponent>()->position;
    auto* pChildRotation = &child->get<RotationComponent>()->rotation;

    *pChildPosition = *parentPosition;
    *pChildRotation = *parentRotation;
}