#include"collision.h"

#include<cfloat>

#include"physics_manager.h"
#include"scene/components.h"

#include"gjk_epa.h"

#include"renderer/render_manager.h" // temp


CollisionPoints CollisionManager::collideSphereSphere(ColliderSphere* pFirstCollider, TransformComponent* pFirstTransform, ColliderSphere* pSecondCollider, TransformComponent* pSecondTransform)
{
    RV_ASSERT(pFirstCollider->relativeRadius - 1.f < 0.0001f, ""); //temp
    RV_ASSERT(pSecondCollider->relativeRadius - 1.f < 0.0001f, ""); //temp

    RV_ASSERT((pFirstTransform->scale.get(0, 0) == pFirstTransform->scale.get(1, 0)) && (pFirstTransform->scale.get(1, 0)  == pFirstTransform->scale.get(2, 0)), "is not actually a sphere");
    RV_ASSERT((pSecondTransform->scale.get(0, 0) == pSecondTransform->scale.get(1, 0)) && (pSecondTransform->scale.get(1, 0)  == pSecondTransform->scale.get(2, 0)), "is not actually a sphere");


    CollisionPoints collisionPoints;
    if(module(pFirstTransform->position - pSecondTransform->position) <= pFirstTransform->scale.get(0, 0) + pSecondTransform->scale.get(0, 0))
    {
        collisionPoints.hasCollided = true;
    }
    else
    {
        collisionPoints.hasCollided = false;
    }

    return collisionPoints;
}

Vec3 ColliderSphere::findFurthestPoint(const Vec3& direction, TransformComponent* pTransform) const
{
    RV_ASSERT((pTransform->scale.get(0, 0) == pTransform->scale.get(1, 0)) && (pTransform->scale.get(1, 0)  == pTransform->scale.get(2, 0)), "is not actually a sphere");

    return direction * relativeRadius * pTransform->scale.get(0, 0);
}

Vec3 ColliderBox::findFurthestPoint(const Vec3& direction, TransformComponent* pTransform) const
{
    RV_ASSERT(false, ""); // temp, can be implemented
}

Vec3 ColliderMesh::findFurthestPoint(const Vec3& direction, TransformComponent* pTransform) const
{
    RV_ASSERT(pMesh != nullptr, "mesh pointer not set");

    Mat<4,4> worldMatrix = pTransform->getTransform();

    Vec3 result;

    float max = -FLT_MAX;
    for(int i=0; i<pMesh->m_Vertices.size(); i++) // potential optimization: put all vertex data in memory consequently for better cache coherency
    {
        Vec4 tempPosVec4 = worldMatrix * Vec4(pMesh->m_Vertices[i].position, 1.f);
        Vec3 tempPosVec3 = {tempPosVec4.get(0, 0), tempPosVec4.get(1, 0), tempPosVec4.get(2, 0)};
        float tempDot = dot(tempPosVec3, direction);
        if(tempDot > max)
        {
            max = tempDot;
            result = tempPosVec3;
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
    return gjkEpa::doGjkBool(pFirstCollider, pFirstTransform, pSecondCollider, pSecondTransform);
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