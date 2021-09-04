#include"collision_manager.h"

#include<cfloat>

#include"physics_manager.h"
#include"scene/components.h"

#include"gjk_epa.h"

void CollisionManager::onUpdateDetectCollisions(float dt)
{
    //detectCollisionsBroadPhase(dt);
    detectCollisionsNarrowPhase(dt);
}

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

            //cout << "Checking collision for: " << pFirst->entityName << " | " << pSecond->entityName << endl;

            CollisionPoints collisionPoints = pColliderFirst->collide(pColliderSecond, pFirst->get<TransformComponent>(), pSecond->get<TransformComponent>());
            if(collisionPoints.hasCollided)
            {
                cout << "collision detected" << endl;
                //RV_ASSERT(false, ""); //temp
                Entity* pDebugLine = Scene::getEntity("DebugLine");
                pDebugLine->get<TransformComponent>()->position = {7, 5, 3};
                pDebugLine->get<TransformComponent>()->scale = collisionPoints.normal;
                pDebugLine->get<TransformComponent>()->rotation.yaw = degreesToRadians(90);
                //pDebugLine->get<TransformComponent>()->rotation = mat::
            }
            Collision collision;
            collision.pEntity1 = pFirst;
            collision.pEntity2 = pSecond;
            collision.collisionPoints = collisionPoints; //TODO: unnecessaraly creating collisionPoints and collision, and also unnecessarily copying collisionPoints to collision

            if(collision.collisionPoints.hasCollided)
            {
                collision.pEntity1->get<ModelComponent>()->model.pMaterials[0]->set("u_Diffuse", Vec3f(1, 0, 0));
                collision.pEntity2->get<ModelComponent>()->model.pMaterials[0]->set("u_Diffuse", Vec3f(1, 0, 0));
                cout << "collision depth: " << collision.collisionPoints.depth << endl;
                //collision.pEntity2->get<TransformComponent>()->position += (collision.collisionPoints.depth + 0.00001f) * collision.collisionPoints.normal;
            }
            else {
                collision.pEntity1->get<ModelComponent>()->model.pMaterials[0]->set("u_Diffuse", Vec3f(0, 1, 0));
                collision.pEntity2->get<ModelComponent>()->model.pMaterials[0]->set("u_Diffuse", Vec3f(0, 1, 0));
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

Vec3f ColliderSphere::findFurthestPoint(const Vec3f& direction, TransformComponent* pTransform) const
{
    RV_ASSERT((pTransform->scale.a[0] == pTransform->scale.a[1]) && (pTransform->scale.a[1]  == pTransform->scale.a[2]), "is not actually a sphere");

    return direction * relativeRadius * pTransform->scale.a[0];
}

Vec3f ColliderBox::findFurthestPoint(const Vec3f& direction, TransformComponent* pTransform) const
{
    RV_ASSERT(false, ""); // temp, can be implemented
}

Vec3f ColliderMesh::findFurthestPoint(const Vec3f& direction, TransformComponent* pTransform) const
{
    RV_ASSERT(pMesh != nullptr, "mesh pointer not set");

    Mat4 worldMatrix = pTransform->getTransform();

    Vec3f result;

    float max = -FLT_MAX;
    for(int i=0; i<pMesh->m_Vertices.size(); i++) // potential optimization: put all vertex data in memory consequently for better cache coherency
    {
        Vec4f tempPosVec4f = worldMatrix * Vec4f(pMesh->m_Vertices[i].position, 1.f);
        Vec3f tempPosVec3f = {tempPosVec4f.a[0], tempPosVec4f.a[1], tempPosVec4f.a[2]};
        float tempDot = dot(tempPosVec3f, direction);
        if(tempDot > max)
        {
            max = tempDot;
            result = tempPosVec3f;
        }
    }

    return result;
}

CollisionPoints CollisionManager::collideSphereBox(ColliderSphere* pFirstCollider, TransformComponent* pFirstTransform, ColliderBox* pSecondCollider, TransformComponent* pSecondTransform)
{
    RV_ASSERT(false, ""); // to be implemented
}

CollisionPoints CollisionManager::collideSphereMesh(ColliderSphere* pFirstCollider, TransformComponent* pFirstTransform, ColliderMesh* pSecondCollider, TransformComponent* pSecondTransform)
{
    RV_ASSERT(false, ""); // to be implemented
}

CollisionPoints CollisionManager::collideBoxMesh(ColliderBox* pFirstCollider, TransformComponent* pFirstTransform, ColliderMesh* pSecondCollider, TransformComponent* pSecondTransform)
{
    RV_ASSERT(false, ""); // to be implemented
}

CollisionPoints CollisionManager::collideBoxBox(ColliderBox* pFirstCollider, TransformComponent* pFirstTransform, ColliderBox* pSecondCollider, TransformComponent* pSecondTransform)
{
    RV_ASSERT(false, ""); // to be implemented
}

CollisionPoints CollisionManager::collideMeshMesh(ColliderMesh* pFirstCollider, TransformComponent* pFirstTransform, ColliderMesh* pSecondCollider, TransformComponent* pSecondTransform)
{
    return gjkEpa::doGjkDetectCollision(pFirstCollider, pFirstTransform, pSecondCollider, pSecondTransform);
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
    RV_ASSERT(pMesh != nullptr, "mesh pointer not set");
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

// -


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

// -

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