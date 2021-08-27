#include"physics_manager.h"

#include"renderer/camera.h"

#include"scene/scene.h"
#include"scene/components.h"

void PhysicsManager::iOnUpdate(float dt)
{
    alignPositionAndRotation(*Scene::getPlayerEntity(), Scene::getCameraEntity());

    onUpdateDynamics(dt);
    onUpdateDetectCollisions(dt);
    onUpdateResolveCollisions(dt);
}

void PhysicsManager::alignPositionAndRotation(const Entity& parent, Entity* child)
{
    RV_ASSERT(parent.has<TransformComponent>() && child->has<TransformComponent>(), "there is no transform, it is required")

    *child->get<TransformComponent>() = *parent.get<TransformComponent>();
}

void PhysicsManager::onUpdateDetectCollisions(float dt)
{
        //detectCollisionsBroadPhase(dt);
        detectCollisionsNarrowPhase(dt);
}

void PhysicsManager::onUpdateDynamics(float dt)
{
    for(auto itEntity = Scene::getEntityList()->begin(); itEntity != Scene::getEntityList()->end(); itEntity++)
    {
        if(!itEntity->valid)
            continue;

        if(!itEntity->has<PhysicalComponent>())
            continue;

        auto* pPhysical = &itEntity->get<PhysicalComponent>()->physical;
        TransformComponent* pTransform = itEntity->get<TransformComponent>();

        cout << "onUpdateDynamics() for entity: " << itEntity->entityName << endl;

        pPhysical->force += pPhysical->gravity * pPhysical->mass * Vec3f(0, 0, -1);

        pPhysical->velocity += pPhysical->force * dt;
        pTransform->position += pPhysical->velocity * dt;

        pPhysical->force = {0, 0, 0};
    }
}

void PhysicsManager::detectCollisionsNarrowPhase(float dt)
{

}

void PhysicsManager::onUpdateResolveCollisions(float dt)
{

}

void PhysicsManager::iInit()
{
}

//PhysicalStatic::PhysicalStatic(TransformComponent* pTransformComponent)
//    : pTransform(pTransformComponent)
//{ }

PhysicalDynamic::PhysicalDynamic() // (TransformComponent* pTransformComponent)
    : mass(1.f), velocity{0, 0, 0}, force{0, 0, 0} // PhysicalStatic(pTransformComponent)
{
    gravity = Scene::getGravity();

    //RV_ASSERT(pTransform != nullptr, "pTransform not set");
    RV_ASSERT(gravity > 1.0, ""); // temp, can be removed
}

void log(const PhysicalDynamic& physical)
{
    cout << "mass: " << physical.mass << endl;

    cout << "velocity: ";
    log(physical.velocity);

    cout << "force: ";
    log(physical.force);

    cout << "gravity: " << physical.gravity << endl;
}