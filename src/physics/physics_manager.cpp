#include"physics_manager.h"
#include"renderer/camera.h"

#include"scene/scene.h"
#include"scene/components.h"

extern Entity* stanic;

void PhysicsManager::updateTransforms() // updates the transforms of all the entities according to the positions and stuff
{
    for(auto itEntity = Scene::getEntityList()->begin(); itEntity != Scene::getEntityList()->end(); itEntity++)
    {
        if(itEntity->has<TransformComponent>() and itEntity->has<PositionComponent>())
        {
            //cout << "Updating tranform for entity: " << itEntity->entityName << endl;
            *itEntity->get<TransformComponent>() = translate(identity, *itEntity->get<PositionComponent>());
        }
    }
}

void PhysicsManager::alignPositionAndRotation(const Entity& parent, Entity* child)
{

    assert(parent.has<PositionComponent>() // ERROR: Required components are not present in entity\n
        and parent.has<RotationComponent>()
        and child->has<PositionComponent>()
        and child->has<RotationComponent>()); 

    auto parentPosition = parent.get<PositionComponent>();
    auto parentRotation = parent.get<RotationComponent>();
    auto childPosition = child->get<PositionComponent>();
    auto childRotation = child->get<RotationComponent>();

    childPosition = parentPosition;
    childRotation = parentRotation;
}

void PhysicsManager::update()
{
    PositionComponent* stanicPos = stanic->get<PositionComponent>();
    auto* player = Scene::getPlayerEntity();

    auto* playerPos = Scene::getPlayerEntity()->get<PositionComponent>();
    *stanicPos = add(*playerPos, Vec3f(5 * sin(glfwGetTime()), 0, 5 * cos(glfwGetTime())));

    alignPositionAndRotation(*Scene::getPlayerEntity(), Scene::getCameraEntity());

    updateTransforms();
}