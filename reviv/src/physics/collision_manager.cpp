#include"collision_manager.h"
#include"physics_manager.h"
#include"scene/components.h"

void CollisionManager::onUpdateDetectCollisions(float dt)
{
    //detectCollisionsBroadPhase(dt);
    detectCollisionsNarrowPhase(dt);
}

//void CollisionManager::detectCollisionsBroadPhase(float dt)
//{
//
//}

void CollisionManager::detectCollisionsNarrowPhase(float dt)
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

            cout << "Checking collision for: " << pFirst->entityName << " | " << pSecond->entityName << endl;

            CollisionPoints collisionPoints = pColliderFirst->collide(pColliderSecond, pFirst->get<TransformComponent>(), pSecond->get<TransformComponent>());
            if(collisionPoints.hasCollided)
            {
                cout << "HAS INDEED COLLIED NIGGA" << endl;
            }

        }
    }
}

CollisionPoints CollisionManager::collideSphereSphere(ColliderSphere* pFirstCollider, TransformComponent* pFirstTransform, ColliderSphere* pSecondCollider, TransformComponent* pSecondTransform)
{
    RV_ASSERT(pFirstCollider->relativeRadius - 1.f < 0.0001f, ""); //temp
    RV_ASSERT(pSecondCollider->relativeRadius - 1.f < 0.0001f, ""); //temp

    RV_ASSERT((pFirstTransform->scale.a[0] == pFirstTransform->scale.a[1]) && (pFirstTransform->scale.a[1]  == pFirstTransform->scale.a[2]), "is not actually a sphere");
    RV_ASSERT((pSecondTransform->scale.a[0] == pSecondTransform->scale.a[1]) && (pSecondTransform->scale.a[1]  == pSecondTransform->scale.a[2]), "is not actually a sphere");


    CollisionPoints collisionPoints;
    if(module(pFirstTransform->position - pSecondTransform->position) <= pFirstTransform->scale.a[0] + pSecondTransform->scale.a[0])
    {
        collisionPoints.hasCollided = true;
    }
    else
    {
        collisionPoints.hasCollided = false;
    }

    return collisionPoints;
}

CollisionPoints CollisionManager::collideSphereBox(ColliderSphere* pFirstCollider, TransformComponent* pFirstTransform, ColliderBox* pSecondCollider, TransformComponent* pSecondTransform)
{
    RV_ASSERT(false, ""); // temp

}

CollisionPoints CollisionManager::collideSphereMesh(ColliderSphere* pFirstCollider, TransformComponent* pFirstTransform, ColliderMesh* pSecondCollider, TransformComponent* pSecondTransform)
{
    RV_ASSERT(false, ""); // temp

}

CollisionPoints CollisionManager::collideBoxMesh(ColliderBox* pFirstCollider, TransformComponent* pFirstTransform, ColliderMesh* pSecondCollider, TransformComponent* pSecondTransform)
{
    RV_ASSERT(false, ""); // temp

}

CollisionPoints CollisionManager::collideBoxBox(ColliderBox* pFirstCollider, TransformComponent* pFirstTransform, ColliderBox* pSecondCollider, TransformComponent* pSecondTransform)
{
    RV_ASSERT(false, ""); // temp

}

CollisionPoints CollisionManager::collideMeshMesh(ColliderMesh* pFirstCollider, TransformComponent* pFirstTransform, ColliderMesh* pSecondCollider, TransformComponent* pSecondTransform)
{
    RV_ASSERT(false, ""); // temp
}


void CollisionManager::onUpdateResolveCollisions(float dt)
{

}

void log(const Collider& collider)
{ }

void log(const ColliderSphere& collider)
{
    cout << "relatvieRadius: " << collider.relativeRadius << endl;
    log((Collider&)collider);
}

void log(const ColliderBox& collider)
{
    log((Collider&)collider);
}

CollisionPoints ColliderSphere::collide(Collider* pSecondCollider, TransformComponent* pFirstTransform, TransformComponent* pSecondTransform)
{
    return pSecondCollider->collideSphere(this, pFirstTransform, pSecondTransform);
}

CollisionPoints ColliderBox::collide(Collider* pSecondCollider, TransformComponent* pFirstTransform, TransformComponent* pSecondTransform)
{
    return pSecondCollider->collideBox(this, pFirstTransform, pSecondTransform);
}

CollisionPoints ColliderMesh::collide(Collider* pSecondCollider, TransformComponent* pFirstTransform, TransformComponent* pSecondTransform)
{
    return pSecondCollider->collideMesh(this, pFirstTransform, pSecondTransform);
}

// ------------

CollisionPoints ColliderSphere::collideSphere(ColliderSphere* pFirstCollider, TransformComponent* pFirstTransform, TransformComponent* pSecondTransform)
{
    return CollisionManager::collideSphereSphere(this, pSecondTransform, pFirstCollider, pFirstTransform);
}

CollisionPoints ColliderSphere::collideBox(ColliderBox* pFirstCollider, TransformComponent* pFirstTransform, TransformComponent* pSecondTransform)
{
    return CollisionManager::collideSphereBox(this, pSecondTransform, pFirstCollider, pFirstTransform);
}

CollisionPoints ColliderSphere::collideMesh(ColliderMesh* pFirstCollider, TransformComponent* pFirstTransform, TransformComponent* pSecondTransform)
{
    return CollisionManager::collideSphereMesh(this, pSecondTransform, pFirstCollider, pFirstTransform);
}




CollisionPoints ColliderBox::collideSphere(ColliderSphere* pFirstCollider, TransformComponent* pFirstTransform, TransformComponent* pSecondTransform)
{
    return CollisionManager::collideSphereBox(pFirstCollider, pFirstTransform, this, pSecondTransform);
}

CollisionPoints ColliderBox::collideBox(ColliderBox* pFirstCollider, TransformComponent* pFirstTransform, TransformComponent* pSecondTransform)
{
    return CollisionManager::collideBoxBox(this, pSecondTransform, pFirstCollider, pFirstTransform);
}

CollisionPoints ColliderBox::collideMesh(ColliderMesh* pFirstCollider, TransformComponent* pFirstTransform, TransformComponent* pSecondTransform)
{
    return CollisionManager::collideBoxMesh(this, pSecondTransform, pFirstCollider, pFirstTransform);
}


CollisionPoints ColliderMesh::collideSphere(ColliderSphere* pFirstCollider, TransformComponent* pFirstTransform, TransformComponent* pSecondTransform)
{
    return CollisionManager::collideSphereMesh(pFirstCollider, pFirstTransform, this, pSecondTransform);
}

CollisionPoints ColliderMesh::collideBox(ColliderBox* pFirstCollider, TransformComponent* pFirstTransform, TransformComponent* pSecondTransform)
{
    return CollisionManager::collideBoxMesh(pFirstCollider, pFirstTransform, this, pSecondTransform);
}

CollisionPoints ColliderMesh::collideMesh(ColliderMesh* pFirstCollider, TransformComponent* pFirstTransform, TransformComponent* pSecondTransform)
{
    return CollisionManager::collideMeshMesh(pFirstCollider, pFirstTransform, this, pSecondTransform);
}
