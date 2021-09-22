#include"collision.h"

#include<cfloat>

#include"physics_manager.h"
#include"scene/components.h"

#include"gjk_epa.h"

#include"renderer/render_manager.h" // temp


CollisionPoints CollisionManager::collideSphereSphere(const ColliderSphere* pFirstCollider, const TransformComponent* pFirstTransform, const ColliderSphere* pSecondCollider, const TransformComponent* pSecondTransform)
{
    RV_ASSERT((pFirstTransform->getScale().get(0, 0) == pFirstTransform->getScale().get(1, 0)) && (pFirstTransform->getScale().get(1, 0)  == pFirstTransform->getScale().get(2, 0)), "is not actually a sphere");
    RV_ASSERT((pSecondTransform->getScale().get(0, 0) == pSecondTransform->getScale().get(1, 0)) && (pSecondTransform->getScale().get(1, 0)  == pSecondTransform->getScale().get(2, 0)), "is not actually a sphere");


    CollisionPoints collisionPoints;
    float distance = module(pFirstTransform->getPosition() - pSecondTransform->getPosition());
    float depth = distance - pFirstTransform->getScale().get(0, 0) + pSecondTransform->getScale().get(0, 0);
    if(depth < 0)
    {
        collisionPoints.hasCollided = true;
        collisionPoints.depth = depth;
        //collisionPoints.pFirst = pFirst->
        assert(false); // TODO: testiraj + normal vector itd
    }
    else
    {
        collisionPoints.hasCollided = false;
    }

    return collisionPoints;
}

Vec3 ColliderSphere::findFurthestPoint(const Vec3& direction, const TransformComponent* pTransform) const
{
    RV_ASSERT((pTransform->getScale().get(0, 0) == pTransform->getScale().get(1, 0)) && (pTransform->getScale().get(1, 0)  == pTransform->getScale().get(2, 0)), "is not actually a sphere");

    return pTransform->getPosition() + normalize(direction) * pTransform->getScale().get(0, 0);
}

Vec3 ColliderBox::findFurthestPoint(const Vec3& direction, const TransformComponent* pTransform) const
{
    RV_ASSERT(false, ""); // temp, can be implemented
}

Vec3 ColliderMesh::findFurthestPoint(const Vec3& direction, const TransformComponent* pTransform) const
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

CollisionPoints CollisionManager::collideSphereBox(const ColliderSphere* pFirstCollider, const TransformComponent* pFirstTransform, const ColliderBox* pSecondCollider, const TransformComponent* pSecondTransform)
{
    RV_ASSERT(false, ""); // to be implemented
}

CollisionPoints CollisionManager::collideSphereMesh(const ColliderSphere* pFirstCollider, const TransformComponent* pFirstTransform, const ColliderMesh* pSecondCollider, const TransformComponent* pSecondTransform)
{
    return gjkEpa::doGjkBool(pFirstCollider, pFirstTransform, pSecondCollider, pSecondTransform);
}

CollisionPoints CollisionManager::collideBoxMesh(const ColliderBox* pFirstCollider, const TransformComponent* pFirstTransform, const ColliderMesh* pSecondCollider, const TransformComponent* pSecondTransform)
{
    RV_ASSERT(false, ""); // to be implemented
}

CollisionPoints CollisionManager::collideBoxBox(const ColliderBox* pFirstCollider, const TransformComponent* pFirstTransform, const ColliderBox* pSecondCollider, const TransformComponent* pSecondTransform)
{
    RV_ASSERT(false, ""); // to be implemented
}

CollisionPoints CollisionManager::collideMeshMesh(const ColliderMesh* pFirstCollider, const TransformComponent* pFirstTransform, const ColliderMesh* pSecondCollider, const TransformComponent* pSecondTransform)
{
    return gjkEpa::doGjkBool(pFirstCollider, pFirstTransform, pSecondCollider, pSecondTransform);
}

void log(const Collider& collider)
{ }

void log(const ColliderSphere& collider)
{
    //cout << "relatvieRadius: " << collider.relativeRadius << endl;
    log((Collider&)collider);
}

void log(const ColliderBox& collider)
{
    log((Collider&)collider);
}

CollisionPoints ColliderSphere::collide(const Collider* pSecondCollider, const TransformComponent* pFirstTransform, const TransformComponent* pSecondTransform) const
{
    return pSecondCollider->collideSphere(this, pFirstTransform, pSecondTransform);
}

CollisionPoints ColliderBox::collide(const Collider* pSecondCollider, const TransformComponent* pFirstTransform, const TransformComponent* pSecondTransform) const
{
    return pSecondCollider->collideBox(this, pFirstTransform, pSecondTransform);
}

CollisionPoints ColliderMesh::collide(const Collider* pSecondCollider, const TransformComponent* pFirstTransform, const TransformComponent* pSecondTransform) const
{
    RV_ASSERT(pMesh != nullptr, "mesh pointer not set");
    return pSecondCollider->collideMesh(this, pFirstTransform, pSecondTransform);
}

// ------------

CollisionPoints ColliderSphere::collideSphere(const ColliderSphere* pFirstCollider, const TransformComponent* pFirstTransform, const TransformComponent* pSecondTransform) const
{
    return CollisionManager::collideSphereSphere(this, pSecondTransform, pFirstCollider, pFirstTransform);
}

CollisionPoints ColliderSphere::collideBox(const ColliderBox* pFirstCollider, const TransformComponent* pFirstTransform, const TransformComponent* pSecondTransform) const
{
    return CollisionManager::collideSphereBox(this, pSecondTransform, pFirstCollider, pFirstTransform);
}

CollisionPoints ColliderSphere::collideMesh(const ColliderMesh* pFirstCollider, const TransformComponent* pFirstTransform, const TransformComponent* pSecondTransform) const
{
    return CollisionManager::collideSphereMesh(this, pSecondTransform, pFirstCollider, pFirstTransform);
}

// -


CollisionPoints ColliderBox::collideSphere(const ColliderSphere* pFirstCollider, const TransformComponent* pFirstTransform, const TransformComponent* pSecondTransform) const
{
    return CollisionManager::collideSphereBox(pFirstCollider, pFirstTransform, this, pSecondTransform);
}

CollisionPoints ColliderBox::collideBox(const ColliderBox* pFirstCollider, const TransformComponent* pFirstTransform, const TransformComponent* pSecondTransform) const
{
    return CollisionManager::collideBoxBox(this, pSecondTransform, pFirstCollider, pFirstTransform);
}

CollisionPoints ColliderBox::collideMesh(const ColliderMesh* pFirstCollider, const TransformComponent* pFirstTransform, const TransformComponent* pSecondTransform) const
{
    return CollisionManager::collideBoxMesh(this, pSecondTransform, pFirstCollider, pFirstTransform);
}

// -

CollisionPoints ColliderMesh::collideSphere(const ColliderSphere* pFirstCollider, const TransformComponent* pFirstTransform, const TransformComponent* pSecondTransform) const
{
    return CollisionManager::collideSphereMesh(pFirstCollider, pFirstTransform, this, pSecondTransform);
}

CollisionPoints ColliderMesh::collideBox(const ColliderBox* pFirstCollider, const TransformComponent* pFirstTransform, const TransformComponent* pSecondTransform) const
{
    return CollisionManager::collideBoxMesh(pFirstCollider, pFirstTransform, this, pSecondTransform);
}

CollisionPoints ColliderMesh::collideMesh(const ColliderMesh* pFirstCollider, const TransformComponent* pFirstTransform, const TransformComponent* pSecondTransform) const
{
    return CollisionManager::collideMeshMesh(pFirstCollider, pFirstTransform, this, pSecondTransform);
}