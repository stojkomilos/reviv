#include"collision.h"

#include<cfloat>

#include"physics_manager.h"
#include"scene/components.h"

#include"gjk_epa.h"

#include"renderer/render_manager.h" // temp


namespace collisionImplementation
{

    CollisionPoints collideMeshMesh(const ColliderMesh* pFirstCollider, const ColliderMesh* pSecondCollider)
    {
        return gjkEpa::doGjkBool(pFirstCollider, pSecondCollider);
    }

    CollisionPoints collideSphereSphere(const ColliderSphere* pFirst, const ColliderSphere* pSecond)
    {
        CollisionPoints collisionPoints;

        float distance = module(pFirst->getTransformComponent().getPosition() - pSecond->getTransformComponent().getPosition());
        float depth = -(distance - pFirst->getTransformComponent().getScale().get(0, 0) - pSecond->getTransformComponent().getScale().get(0, 0));

        if(depth > 0)
        {
            collisionPoints.hasCollided = true;
            collisionPoints.depth = depth;

            Vec3 normal = normalize(pFirst->getTransformComponent().getPosition() - pSecond->getTransformComponent().getPosition());

            collisionPoints.firstPoint = pSecond->getTransformComponent().getPosition() + normal * (pSecond->getTransformComponent().getScale().get(0, 0) - depth);
            collisionPoints.secondPoint = pFirst->getTransformComponent().getPosition() - normal * (pFirst->getTransformComponent().getScale().get(0, 0) - depth);

            //cout << "depth :" << collisionPoints.depth << endl;
        }
        else
        {
            collisionPoints.hasCollided = false;
        }

        return collisionPoints;
    }

    CollisionPoints collideSphereBox(const ColliderSphere* pFirstCollider, const ColliderBox* pSecondCollider)
    {
        RV_ASSERT(false, ""); // to be implemented
    }

    CollisionPoints collideSphereMesh(const ColliderSphere* pFirstCollider, const ColliderMesh* pSecondCollider)
    {
        return gjkEpa::doGjkBool(pFirstCollider, pSecondCollider);
    }

    CollisionPoints collideBoxMesh(const ColliderBox* pFirstCollider, const ColliderMesh* pSecondCollider)
    {
        RV_ASSERT(false, ""); // to be implemented
    }

    CollisionPoints collideBoxBox(const ColliderBox* pFirstCollider, const ColliderBox* pSecondCollider)
    {
        RV_ASSERT(false, ""); // to be implemented
    }
}

Vec3 ColliderSphere::findFurthestPoint(const Vec3& direction) const
{
    return pTransformComponent->getPosition() + normalize(direction) * pTransformComponent->getScale().get(0, 0);
}

Vec3 ColliderBox::findFurthestPoint(const Vec3& direction) const
{
    RV_ASSERT(false, ""); // temp, can be implemented
}

Vec3 ColliderMesh::findFurthestPoint(const Vec3& direction) const
{
    RV_ASSERT(pMesh != nullptr, "mesh pointer not set");

    Mat<4,4> worldMatrix = pTransformComponent->getTransform();

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

CollisionPoints ColliderSphere::collide(const Collider* pSecondCollider) const
{
    return pSecondCollider->collideSphere(this);
}

CollisionPoints ColliderBox::collide(const Collider* pSecondCollider) const
{
    return pSecondCollider->collideBox(this);
}

CollisionPoints ColliderMesh::collide(const Collider* pSecondCollider) const
{
    RV_ASSERT(pMesh != nullptr, "mesh pointer not set");
    return pSecondCollider->collideMesh(this);
}

// ------------

CollisionPoints ColliderSphere::collideSphere(const ColliderSphere* pFirstCollider) const
{
    return collisionImplementation::collideSphereSphere(this, pFirstCollider);
}

CollisionPoints ColliderSphere::collideBox(const ColliderBox* pFirstCollider) const
{
    return collisionImplementation::collideSphereBox(this, pFirstCollider);
}

CollisionPoints ColliderSphere::collideMesh(const ColliderMesh* pFirstCollider) const
{
    return collisionImplementation::collideSphereMesh(this, pFirstCollider);
}

// -


CollisionPoints ColliderBox::collideSphere(const ColliderSphere* pFirstCollider) const
{
    return collisionImplementation::collideSphereBox(pFirstCollider, this);
}

CollisionPoints ColliderBox::collideBox(const ColliderBox* pFirstCollider) const
{
    return collisionImplementation::collideBoxBox(this, pFirstCollider);
}

CollisionPoints ColliderBox::collideMesh(const ColliderMesh* pFirstCollider) const
{
    return collisionImplementation::collideBoxMesh(this, pFirstCollider);
}

// -

CollisionPoints ColliderMesh::collideSphere(const ColliderSphere* pFirstCollider) const
{
    return collisionImplementation::collideSphereMesh(pFirstCollider, this);
}

CollisionPoints ColliderMesh::collideBox(const ColliderBox* pFirstCollider) const
{
    return collisionImplementation::collideBoxMesh(pFirstCollider, this);
}

CollisionPoints ColliderMesh::collideMesh(const ColliderMesh* pFirstCollider) const
{
    return collisionImplementation::collideMeshMesh(pFirstCollider, this);
}