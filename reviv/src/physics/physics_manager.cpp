#include"physics_manager.h"

#include"renderer/camera.h"

#include"scene/scene.h"
#include"scene/components.h"

#include"core/input.h"

void PhysicsManager::onUpdate(float dt)
{
    alignPositionAndRotation(*Scene::getPlayerEntity(), Scene::getCameraEntity());

    constraintsCollision.clear();

    precalculateVelocitiesAndForces(dt);
    doCollisionDetection(dt);
    satisfyConstraintsCollision(dt);
    satisfyConstraintsGeneral(dt);
    calculateNewPositionsVelocitiesAndForces(dt);
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

void PhysicsManager::precalculateVelocitiesAndForces(float dt)
{
    for(auto itEntity = Scene::getEntityList()->begin(); itEntity != Scene::getEntityList()->end(); itEntity++)
    {
        if(!Scene::isEntityValid(&(*itEntity)))
            continue;
        if(!itEntity->has<PhysicalComponent>())
            continue;

        auto* pPhysical = &itEntity->get<PhysicalComponent>()->physical;
        auto* pTransform = itEntity->get<TransformComponent>();

        pPhysical->force += pPhysical->getMass() * pPhysical->gravity * Vec3(0, 0, -1);
    }
}

void PhysicsManager::satisfyConstraintsCollision(float dt)
{
    for(int i=0; i<constraintsCollision.size(); i++)
    {
        cout << "constraintCollision " << i << "/" << constraintsCollision.size()-1 << " between: " << constraintsCollision[i].pFirst->entityName << " and: " << constraintsCollision[i].pSecond->entityName << endl;
        RV_ASSERT((&(constraintsCollision[i]))->getType() == Constraint::ConstraintType::PENETRATION, "non penetration/collision constraint. only penetration/collision constraints should be in this array");

        //constraintsCollision[i].pSecond->get<TransformComponent>()->position += (constraintsCollision[i].collisionPoints.depth + 0.0001f) * constraintsCollision[i].collisionPoints.normal;
        constraintsCollision[i].solve(dt);
    }
}

void PhysicsManager::satisfyConstraintsGeneral(float dt)
{
    for(int i=0; i<constraintsGeneral.size(); i++)
    {
        RV_ASSERT(constraintsGeneral[i]->getType() != Constraint::ConstraintType::PENETRATION, "all the penetration/collision constraints should have been resolved in satisfyConstraintsCollision()");
        constraintsGeneral[i]->solve(dt);
    }
}

void PhysicsManager::calculateNewPositionsVelocitiesAndForces(float dt)
{

    for(auto itEntity = Scene::getEntityList()->begin(); itEntity != Scene::getEntityList()->end(); itEntity++)
    {
        if(!Scene::isEntityValid(&(*itEntity)))
            continue;
        if(!itEntity->has<PhysicalComponent>())
            continue;


        auto* pPhysical = &itEntity->get<PhysicalComponent>()->physical;
        auto* pTransform = itEntity->get<TransformComponent>();

        if(pPhysical->fixedTranslation == false)
        {
            *pTransform->getPositionPtr() += pPhysical->velocity * dt;

            pPhysical->velocity += pPhysical->force / pPhysical->getMass() * dt;

            pPhysical->force = Vec3(0, 0, 0);
        }

        if(pPhysical->fixedRotation == false)
        {
            *pTransform->getRotationPtr()->getPtr(0, 0) += pPhysical->angularVelocity.get(0, 0) * dt;
            *pTransform->getRotationPtr()->getPtr(1, 0) += pPhysical->angularVelocity.get(1, 0) * dt;
            *pTransform->getRotationPtr()->getPtr(2, 0) += pPhysical->angularVelocity.get(2, 0) * dt;

            pPhysical->angularVelocity += *pPhysical->getInverseInertiaTensor() * pPhysical->torque * dt;

            pPhysical->torque = Vec3(0, 0, 0);
        }
    }
}


void PhysicsManager::doCollisionDetection(float dt)
{
    doCollisionDetectionNarrowPhase(dt);
}

PhysicalDynamic::PhysicalDynamic(float mass /*= 1f*/ )
    : velocity{0, 0, 0}, angularVelocity{0, 0, 0}, force{0, 0, 0}, torque{0, 0, 0}
{
    gravity = Scene::getGravity();

    setMass(mass);
    inertiaTensor.setToIdentity();      // TODO: both
    inverseInertiaTensor.setToIdentity();
}

void PhysicsManager::doCollisionDetectionNarrowPhase(float dt)
{
    for(unsigned int i=0; i<Scene::getEntityList()->size(); i++)
    {

        Entity* pFirst = &(*Scene::getEntityList())[i];

        if(!Scene::isEntityValid(pFirst))
            continue;

        Collider* pColliderFirst = PhysicsManager::get()->getCollidableFromEntity(pFirst);
        if(pColliderFirst == nullptr)
            continue;

        for(unsigned int j=i+1; j<Scene::getEntityList()->size(); j++)
        {
            Entity* pSecond = &(*Scene::getEntityList())[j];

            if(!Scene::isEntityValid(pSecond))
                continue;

            Collider* pColliderSecond = PhysicsManager::get()->getCollidableFromEntity(pSecond);
            if(pColliderSecond == nullptr)
                continue;

            if(PhysicsManager::get()->getCollidableFromEntity(pSecond) == nullptr)
                continue;

            //cout << "Checking collision for: " << pFirst->entityName << " | " << pSecond->entityName << endl;

            CollisionPoints collisionPoints = pColliderFirst->collide(pColliderSecond, pFirst->get<TransformComponent>(), pSecond->get<TransformComponent>());
            if(collisionPoints.hasCollided)
            {
                constraintsCollision.emplace_back();
                constraintsCollision[constraintsCollision.size()-1].collisionPoints = collisionPoints;
                constraintsCollision[constraintsCollision.size()-1].pFirst = pFirst;
                constraintsCollision[constraintsCollision.size()-1].pSecond = pSecond;
            }
                


/*              // Debug
            if(collisionPoints.hasCollided)
            {
                //cout << "collision detected" << endl;

                //collision.pEntity2->get<TransformComponent>()->position += (collision.collisionPoints.depth + 0.00001f) * collision.collisionPoints.normal;
                pFirst->get<ModelComponent>()->model.pMaterials[0]->reset(&RenderManager::get()->shaderBlend);
                pFirst->get<ModelComponent>()->model.pMaterials[0]->reset(&RenderManager::get()->shaderBlend);

                pSecond->get<ModelComponent>()->model.pMaterials[0]->set("u_Color", Vec4(1, 0, 0, 0.65));
                pSecond->get<ModelComponent>()->model.pMaterials[0]->set("u_Color", Vec4(0, 0, 1, 0.65));

                Entity* pDebugLine = Scene::getEntity("DebugLine");
                pDebugLine->get<TransformComponent>()->position = (collisionPoints.firstPoint + collisionPoints.secondPoint) / 2;
                pDebugLine->get<TransformComponent>()->rotation = lookAtGetRotation(Vec3(0, 0, 0), -collisionPoints.normal);
                pDebugLine->get<TransformComponent>()->scale = {collisionPoints.depth, 0.05f, 0.05f};
            }
            else {
                pFirst->get<ModelComponent>()->model.pMaterials[0]->reset(&RenderManager::get()->shaderDefferedGeometry);
                pFirst->get<ModelComponent>()->model.pMaterials[0]->set("u_Diffuse", Vec3(0, 1, 0));
                pSecond->get<ModelComponent>()->model.pMaterials[0]->reset(&RenderManager::get()->shaderDefferedGeometry);
                pSecond->get<ModelComponent>()->model.pMaterials[0]->set("u_Diffuse", Vec3(0, 1, 0));
            }
*/
        }
    }
}

void PhysicalDynamic::setMass(float newMass)
{
    mass = newMass;
    inverseMass = 1.f / newMass;
    // TODO: change moment of inertia
}

void PhysicsManager::init()
{

}

void log(const PhysicalDynamic& physical)
{
    cout << "mass: " << physical.getMass() << endl;

    cout << "velocity: ";
    log(physical.velocity);

    cout << "force: ";
    log(physical.force);
}
