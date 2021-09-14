#include"physics_manager.h"

#include"renderer/camera.h"

#include"scene/scene.h"
#include"scene/components.h"

void PhysicsManager::init()
{

}

void PhysicsManager::onUpdate(float dt)
{
    alignPositionAndRotation(*Scene::getPlayerEntity(), Scene::getCameraEntity());

    calculateNewVelocitiesAndForces(dt);
    iterateConstrainst(dt);
    calculateNewPositionsAndVelocities(dt);
    //DynamicsManager::get()->onUpdate(dt);
    //CollisionManager::get()->onUpdateDetectCollisions(dt);
    //CollisionManager::get()->onUpdateResolveCollisions(dt);
}

void PhysicsManager::alignPositionAndRotation(const Entity& parent, Entity* child)
{
    RV_ASSERT(parent.has<TransformComponent>() && child->has<TransformComponent>(), "there is no transform, it is required")

    *child->get<TransformComponent>() = *parent.get<TransformComponent>();
}

Collider* PhysicsManager::getCollidableFromEntity(Entity* pEntity)
{
    RV_ASSERT(Scene::isEntityValid(pEntity), "entity not valid")
    RV_ASSERT((pEntity->has<ColliderMeshComponent>() && !pEntity->has<ColliderSphereComponent>() && !pEntity->has<ColliderBoxComponent>())
        || (!pEntity->has<ColliderMeshComponent>() && pEntity->has<ColliderSphereComponent>() && !pEntity->has<ColliderBoxComponent>()) 
        || (!pEntity->has<ColliderMeshComponent>() && !pEntity->has<ColliderSphereComponent>() && pEntity->has<ColliderBoxComponent>())
        || (!pEntity->has<ColliderMeshComponent>() && !pEntity->has<ColliderSphereComponent>() && !pEntity->has<ColliderBoxComponent>()), "entity can't have more than one collider component");

    if(pEntity->has<ColliderSphereComponent>())
        return &pEntity->get<ColliderSphereComponent>()->collider;

    if(pEntity->has<ColliderBoxComponent>())
        return &pEntity->get<ColliderBoxComponent>()->collider;

    if(pEntity->has<ColliderMeshComponent>())
        return &pEntity->get<ColliderMeshComponent>()->collider;

    return nullptr;
}

void PhysicsManager::calculateNewVelocitiesAndForces(float dt)
{
    for(auto itEntity = Scene::getEntityList()->begin(); itEntity != Scene::getEntityList()->end(); itEntity++)
    {
        if(!Scene::isEntityValid(&(*itEntity)))
            continue;
        if(!itEntity->has<PhysicalComponent>())
            continue;

        //cout << "Updating dynamics for entity: " << itEntity->entityName << endl;

        auto* pPhysical = &itEntity->get<PhysicalComponent>()->physical;
        auto* pTransform = itEntity->get<TransformComponent>();

        pPhysical->force += pPhysical->mass * pPhysical->gravity * Vec3f(0, 0, -1);

        pPhysical->velocity += pPhysical->force / pPhysical->mass * dt;
        pTransform->position += pPhysical->velocity * dt;

        pPhysical->force = Vec3f(0, 0, 0);
    }
}

void PhysicsManager::iterateConstrainst(float dt)
{
    int nrConstraintIterations = 1;
    MatN velocities(12, 1);
    for(int constraintIteration=0; constraintIteration < nrConstraintIterations; constraintIteration++)
    {
        Entity* pFirst = Scene::getEntity("prvo");
        Entity* pSecond = Scene::getEntity("drugo");
    }
}

void PhysicsManager::calculateNewPositionsAndVelocities(float dt)
{

}

PhysicalDynamic::PhysicalDynamic()
    : mass(1.f), velocity{0, 0, 0}, force{0, 0, 0}
{
    gravity = Scene::getGravity();
}

void log(const PhysicalDynamic& physical)
{
    cout << "mass: " << physical.mass << endl;

    cout << "velocity: ";
    log(physical.velocity);

    cout << "force: ";
    log(physical.force);
}