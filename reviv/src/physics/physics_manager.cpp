#include"physics_manager.h"

#include"renderer/camera.h"

#include"scene/scene.h"
#include"scene/components.h"

void PhysicsManager::iOnUpdate(float dt)
{
    alignPositionAndRotation(*Scene::getPlayerEntity(), Scene::getCameraEntity());

    DynamicsManager::get()->onUpdate(dt);
    CollisionManager::get()->onUpdateDetectCollisions(dt);
    CollisionManager::get()->onUpdateResolveCollisions(dt);
}

void PhysicsManager::alignPositionAndRotation(const Entity& parent, Entity* child)
{
    RV_ASSERT(parent.has<TransformComponent>() && child->has<TransformComponent>(), "there is no transform, it is required")

    *child->get<TransformComponent>() = *parent.get<TransformComponent>();
}


void PhysicsManager::onUpdateResolveCollisions(float dt)
{

}

void PhysicsManager::iInit()
{
}

Collider* PhysicsManager::getCollidableFromEntity(Entity* pEntity)
{
    RV_ASSERT(Scene::isEntityValid(pEntity), "entity not valid")
    RV_ASSERT((/*pEntity->has<ColliderAabbComponent>() &&*/ !pEntity->has<ColliderSphereComponent>() && !pEntity->has<ColliderBoxComponent>())
        || (/*!pEntity->has<ColliderAabbComponent>() &&*/ pEntity->has<ColliderSphereComponent>() && !pEntity->has<ColliderBoxComponent>()) 
        || (/*!pEntity->has<ColliderAabbComponent>() &&*/ !pEntity->has<ColliderSphereComponent>() && pEntity->has<ColliderBoxComponent>())
        || (/*!pEntity->has<ColliderAabbComponent>() &&*/ !pEntity->has<ColliderSphereComponent>() && !pEntity->has<ColliderBoxComponent>()), "entity can't have more than one collider component");

    if(pEntity->has<ColliderSphereComponent>())
        return &pEntity->get<ColliderSphereComponent>()->collider;

    if(pEntity->has<ColliderBoxComponent>())
        return &pEntity->get<ColliderBoxComponent>()->collider;

    return nullptr;
}
