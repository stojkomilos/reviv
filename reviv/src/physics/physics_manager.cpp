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

void PhysicsManager::updateTransforms() // updates the transforms of all the entities according to the positions && stuff
{
    for(auto itEntity = Scene::getEntityList()->begin(); itEntity != Scene::getEntityList()->end(); itEntity++)
    {
        if(itEntity->has<TransformComponent>() && itEntity->has<PositionComponent>())
        {
            auto* pTransform = &itEntity->get<TransformComponent>()->transform;
            //cout << "Updating tranform for entity: " << itEntity->entityName << endl;
            *pTransform = translate(identity, itEntity->get<PositionComponent>()->position);

            if(itEntity->has<RotationComponent>())
            {
                //*pTransform = rotate(*pTransform, itEntity->get<RotationComponent>());
                *pTransform = rotateX(*pTransform, 3.14/2);
            }
        }
    }
}

void PhysicsManager::alignPositionAndRotation(const Entity& parent, Entity* child)
{

    assert(parent.has<PositionComponent>() // ERROR: Required components are not present in entity\n
        && parent.has<RotationComponent>()
        && child->has<PositionComponent>()
        && child->has<RotationComponent>()); 

    auto* parentPosition = &parent.get<PositionComponent>()->position;
    auto* parentRotation = &parent.get<RotationComponent>()->rotation;
    auto* pChildPosition = &child->get<PositionComponent>()->position;
    auto* pChildRotation = &child->get<RotationComponent>()->rotation;

    *pChildPosition = *parentPosition;
    *pChildRotation = *parentRotation;
}