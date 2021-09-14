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
    Entity* pFirst = Scene::getEntity("prvo");
    Entity* pSecond = Scene::getEntity("drugo");

    // set positions

    MatN positions(12, 1);
    *positions.getPtr(0, 0) = pFirst->get<TransformComponent>()->position.a[0];
    *positions.getPtr(1, 0) = pFirst->get<TransformComponent>()->position.a[1];
    *positions.getPtr(2, 0) = pFirst->get<TransformComponent>()->position.a[2];

    *positions.getPtr(3, 0) = 0;
    *positions.getPtr(4, 0) = 0;
    *positions.getPtr(5, 0) = 0;

    *positions.getPtr(6, 0) = pSecond->get<TransformComponent>()->position.a[0];
    *positions.getPtr(7, 0) = pSecond->get<TransformComponent>()->position.a[1];
    *positions.getPtr(8, 0) = pSecond->get<TransformComponent>()->position.a[2];

    *positions.getPtr(9, 0) = 0;
    *positions.getPtr(10, 0) = 0;
    *positions.getPtr(11, 0) = 0;

    // set velocities

    MatN velocities(12, 1);

    *velocities.getPtr(0, 0) = pFirst->get<PhysicalComponent>()->physical.velocity.a[0];
    *velocities.getPtr(1, 0) = pFirst->get<PhysicalComponent>()->physical.velocity.a[1];
    *velocities.getPtr(2, 0) = pFirst->get<PhysicalComponent>()->physical.velocity.a[2];

    *velocities.getPtr(3, 0) = 0;
    *velocities.getPtr(4, 0) = 0;
    *velocities.getPtr(5, 0) = 0;

    *velocities.getPtr(6, 0) = pSecond->get<PhysicalComponent>()->physical.velocity.a[0];
    *velocities.getPtr(7, 0) = pSecond->get<PhysicalComponent>()->physical.velocity.a[1];
    *velocities.getPtr(8, 0) = pSecond->get<PhysicalComponent>()->physical.velocity.a[2];

    *velocities.getPtr(9, 0)  = 0;
    *velocities.getPtr(10, 0) = 0;
    *velocities.getPtr(11, 0) = 0;

    // set masses and moments of inertia
    
    MatN mass(12, 12);

    for(int i=0; i<mass.height; i++)
        for(int j=0; j<mass.width; j++)
            *mass.getPtr(i, j) = 0;

    *mass.getPtr(0, 0) = pFirst->get<PhysicalComponent>()->physical.mass;
    *mass.getPtr(1, 1) = pFirst->get<PhysicalComponent>()->physical.mass;
    *mass.getPtr(2, 2) = pFirst->get<PhysicalComponent>()->physical.mass;

    *mass.getPtr(6, 6) = pSecond->get<PhysicalComponent>()->physical.mass;
    *mass.getPtr(7, 7) = pSecond->get<PhysicalComponent>()->physical.mass;
    *mass.getPtr(8, 8) = pSecond->get<PhysicalComponent>()->physical.mass;

    Vec3f firstPosition = pFirst->get<TransformComponent>()->position;
    Vec3f secondPosition = pSecond->get<TransformComponent>()->position;

    int nrConstraintIterations = 1;
    float constraintDistnace = 5.f;
    for(int constraintIteration=0; constraintIteration < nrConstraintIterations; constraintIteration++)
    {
        Vec3f positionDifference = firstPosition - secondPosition;
        if(abs(dot(positionDifference, positionDifference) - constraintDistnace * constraintDistnace) > 0.00001f)
        {
            cout << "constraint broken. distance: " << module(firstPosition - secondPosition) << endl;
            
        }
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