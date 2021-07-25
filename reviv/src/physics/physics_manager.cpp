#include"physics_manager.h"
#include"renderer/camera.h"

#include"scene/scene.h"
#include"scene/components.h"

//extern Entity* stanic;

void PhysicsManager::iOnUpdate()
{
    alignPositionAndRotation(*Scene::getPlayerEntity(), Scene::getCameraEntity());
}

void PhysicsManager::iInit()
{

}

void PhysicsManager::alignPositionAndRotation(const Entity& parent, Entity* child)
{
    RV_ASSERT(parent.has<TransformComponent>() && child->has<TransformComponent>(), "there is no transform, it is required")

    *child->get<TransformComponent>() = *parent.get<TransformComponent>();
}