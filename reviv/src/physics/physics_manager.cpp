#include"physics_manager.h"

#include"renderer/camera.h"

#include"scene/scene.h"
#include"scene/components.h"

#include"core/input.h"
#include"renderer/render_manager.h"

void PhysicsManager::onUpdate(float dt)
{
    alignPositionAndRotation(*Scene::getPlayerEntity(), Scene::getCameraEntity());

    constraintsCollision.clear();

    precalculateVelocitiesAndForces(dt);
    doCollisionDetection(dt);
        debugDrawingCollisions();
    satisfyConstraintsCollision(dt);
    satisfyConstraintsGeneral(dt);
    calculateNewPositionsVelocitiesAndForces(dt);
}

void PhysicsManager::alignPositionAndRotation(const Entity& parent, Entity* child)
{
    RV_ASSERT(parent.has<TransformComponent>() && child->has<TransformComponent>(), "there is no transform, it is required")

    *child->get<TransformComponent>() = *parent.get<TransformComponent>();
}

void PhysicsManager::precalculateVelocitiesAndForces(float dt)
{
    for(auto itEntity = Scene::getEntityList()->begin(); itEntity != Scene::getEntityList()->end(); itEntity++)
    {
        if(!Scene::isEntityValid(&(*itEntity)))
            continue;
        if(!itEntity->has<RigidbodyComponent>())
            continue;

        auto* pPhysical = &itEntity->get<RigidbodyComponent>()->rigidbody;
        auto* pTransform = itEntity->get<TransformComponent>();

        pPhysical->force += pPhysical->getMass() * pPhysical->gravity * Vec3(0, 0, -1);
    }
}

void PhysicsManager::satisfyConstraintsCollision(float dt)
{
    for(int i=0; i<constraintsCollision.size(); i++)
    {
        //cout << "constraintCollision " << i << "/" << constraintsCollision.size()-1 << " between: " << constraintsCollision[i].pFirst->entityName << " and: " << constraintsCollision[i].pSecond->entityName << endl;
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
        if(!itEntity->has<RigidbodyComponent>())
            continue;


        auto* pPhysical = &itEntity->get<RigidbodyComponent>()->rigidbody;
        auto* pTransform = itEntity->get<TransformComponent>();

        if(pPhysical->fixedTranslation == false)
        {
            pPhysical->velocity += pPhysical->force / pPhysical->getMass() * dt;

            *pTransform->getPositionPtr() += pPhysical->velocity * dt;

            pPhysical->force = Vec3(0, 0, 0);
        }

        if(pPhysical->fixedRotation == false)
        {
            pPhysical->angularVelocity += pPhysical->getInverseInertiaTensor() * pPhysical->torque * dt;

            *pTransform->getRotationPtr() += pPhysical->angularVelocity * dt;

            pPhysical->torque = Vec3(0, 0, 0);
        }
    }
}


void PhysicsManager::doCollisionDetection(float dt)
{
    doCollisionDetectionNarrowPhase(dt);
}

void PhysicsManager::doCollisionDetectionNarrowPhase(float dt)
{
    for(unsigned int i=0; i<Scene::getEntityList()->size(); i++)
    {

        Entity* pFirst = &(*Scene::getEntityList())[i];

        if(!Scene::isEntityValid(pFirst))
            continue;

        if(pFirst->has<RigidbodyComponent>() == false)
            continue;

        Collider* pColliderFirst = pFirst->get<RigidbodyComponent>()->rigidbody.getCollider();
        if(pColliderFirst == nullptr)
            continue;

        for(unsigned int j=i+1; j<Scene::getEntityList()->size(); j++)
        {
            Entity* pSecond = &(*Scene::getEntityList())[j];

            if(!Scene::isEntityValid(pSecond))
                continue;

            if(pSecond->has<RigidbodyComponent>() == false)
                continue;

            Collider* pColliderSecond = pSecond->get<RigidbodyComponent>()->rigidbody.getCollider();
            if(pColliderSecond == nullptr)
                continue;

            //cout << "Checking collision for: " << pFirst->entityName << " | " << pSecond->entityName << endl;

            CollisionPoints collisionPoints = pColliderFirst->collide(pColliderSecond);

            if(collisionPoints.hasCollided)
            {
                RV_ASSERT(collisionPoints.depth >= 0.f, "");
                RV_ASSERT(abs(collisionPoints.depth - module(collisionPoints.firstPoint - collisionPoints.secondPoint)) < 0.02f, "");

                //cout << "Collision detected: " << pFirst->entityName << " and " << pSecond->entityName << endl;

                constraintsCollision.emplace_back();

                float firstRestitution = pFirst->get<RigidbodyComponent>()->rigidbody.restitution;
                float secondRestitution = pSecond->get<RigidbodyComponent>()->rigidbody.restitution;

                constraintsCollision[constraintsCollision.size()-1].restitution = (firstRestitution + secondRestitution) / 2.f;
                constraintsCollision[constraintsCollision.size()-1].collisionPoints = collisionPoints;
                constraintsCollision[constraintsCollision.size()-1].pFirst = pFirst;
                constraintsCollision[constraintsCollision.size()-1].pSecond = pSecond;
            }
        }
    }
}

void PhysicsManager::debugDrawingCollisions()
{
    if(Time::get()->isOneSecond())
        cout << "Collision DEBUG mode ON" << endl;

    for(auto it=Scene::getEntityList()->begin(); it != Scene::getEntityList()->end(); it++)
    {
        //if(it->valid == false) // TODO: temp comment. should not be commented usually
        //    continue;

        if(it->has<ModelComponent>() == false)
            continue;

        if(it->get<ModelComponent>()->model.pMaterials[0]->pShader == &RenderManager::get()->shaderBlend)
            //for(int i=0; i<it->get<ModelComponent>()->model.pMaterials.size(); i++)
            {
                it->get<ModelComponent>()->model.pMaterials[0]->reset(&RenderManager::get()->shaderDefferedGeometry);
                it->get<ModelComponent>()->model.pMaterials[0]->set("u_Diffuse", Vec3(0, 1, 0));
                it->get<ModelComponent>()->model.pMaterials[0]->set("u_Specular", 0.2f);

                it->valid = true; // TODO: temp
            }
    }

    for(int i=0; i<constraintsCollision.size(); i++)
    {
        Entity* pFirst = constraintsCollision[i].pFirst;
        Entity* pSecond = constraintsCollision[i].pSecond;
        RV_ASSERT(constraintsCollision[i].collisionPoints.hasCollided, "");

        //cout << "drawing DEBUG collision for entities: " << pFirst->entityName << " and " << pSecond->entityName << endl;

        pFirst->get<ModelComponent>()->model.pMaterials[0]->reset(&RenderManager::get()->shaderBlend);
        pSecond->get<ModelComponent>()->model.pMaterials[0]->reset(&RenderManager::get()->shaderBlend);

        pFirst->get<ModelComponent>()->model.pMaterials[0]->set("u_Color", Vec4(1, 0, 0, 0.65));
        pSecond->get<ModelComponent>()->model.pMaterials[0]->set("u_Color", Vec4(0, 0, 1, 0.65));

        pFirst->valid = false; // TODO: temp
        pSecond->valid = false; // TODO: temp

        Entity* pDebugLine = Scene::getEntity("DebugLine");
        *pDebugLine->get<TransformComponent>()->getPositionPtr() = (constraintsCollision[i].collisionPoints.firstPoint + constraintsCollision[i].collisionPoints.secondPoint) / 2;
        *pDebugLine->get<TransformComponent>()->getRotationPtr() = lookAtGetRotation(Vec3(0, 0, 0), -(constraintsCollision[i].collisionPoints.firstPoint - constraintsCollision[i].collisionPoints.secondPoint));
        pDebugLine->get<TransformComponent>()->setScale({constraintsCollision[i].collisionPoints.depth, 0.05f, 0.05f});
    }
}

void PhysicsManager::init()
{

}
