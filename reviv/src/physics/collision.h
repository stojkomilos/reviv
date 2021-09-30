#pragma once

#include"core/mat.h"
#include"renderer/mesh.h"

class Entity;

class TransformComponent;

class ColliderSphere;
class ColliderBox;
class ColliderMesh;

class CollisionPoints
{
public:
    Vec3 firstPoint; // deepest point of pFirst in pSecond
    Vec3 secondPoint; // deepest point of pSecond in pFirst
    //Vec3 normal;
    float depth;
    bool hasCollided;
};

class TransformComponent;

enum class ColliderShape { SPHERE=0, MESH, BOX };

class Collider
{
public:

    Collider(const TransformComponent* pTransformComponent)
        : pTransformComponent(pTransformComponent) {};
    virtual ~Collider() = default;

    virtual CollisionPoints collide(const Collider* pSecondCollider) const = 0;
    virtual Vec3 findFurthestPoint(const Vec3& direction) const = 0; // a.k.a. support function. used for gjk collision detection

    virtual CollisionPoints collideSphere(const ColliderSphere* pFirstCollider) const = 0;

    virtual CollisionPoints collideBox(const ColliderBox* pFirstCollider) const = 0;

    virtual CollisionPoints collideMesh(const ColliderMesh* pFirstCollider) const = 0;

    inline const TransformComponent& getTransformComponent() const { return *pTransformComponent; }

protected:
    const TransformComponent* const pTransformComponent = nullptr;

protected:
    Collider() = delete;
};

class ColliderSphere : public Collider
{
public:
    ColliderSphere(const TransformComponent* pTransformComponent)
        : Collider(pTransformComponent) { }

    virtual Vec3 findFurthestPoint(const Vec3& direction) const override;
    virtual CollisionPoints collide(const Collider* pSecondCollider) const override;
    virtual CollisionPoints collideSphere(const ColliderSphere* pFirstCollider) const override;
    virtual CollisionPoints collideBox(const ColliderBox* pFirstCollider) const override;
    virtual CollisionPoints collideMesh(const ColliderMesh* pFirstCollider) const override;

    //float relativeRadius; // the radius gets scaled by the entitys respective transform
};

class ColliderBox : public Collider
{
public:
    ColliderBox(const TransformComponent* pTransformComponent)
        : Collider(pTransformComponent) { }

    virtual Vec3 findFurthestPoint(const Vec3& direction) const override;
    virtual CollisionPoints collide(const Collider* pSecondCollider) const override;
    virtual CollisionPoints collideSphere(const ColliderSphere* pFirstCollider) const override;
    virtual CollisionPoints collideBox(const ColliderBox* pFirstCollider) const override;
    virtual CollisionPoints collideMesh(const ColliderMesh* pFirstCollider) const override;
};

class ColliderMesh : public Collider
{
public:
    ColliderMesh() = delete;

    ColliderMesh(const TransformComponent* pTransformComponent)
        : Collider(pTransformComponent) { }

    virtual Vec3 findFurthestPoint(const Vec3& direction) const override;

    const Mesh* pMesh = nullptr;

    virtual CollisionPoints collide(const Collider* pSecondCollider) const override;
    virtual CollisionPoints collideSphere(const ColliderSphere* pFirstCollider) const override;
    virtual CollisionPoints collideBox(const ColliderBox* pFirstCollider) const override;
    virtual CollisionPoints collideMesh(const ColliderMesh* pFirstCollider) const override;
};

namespace collisionImplementation
{
    //TODO: maybe? place all this in Collider class, remove CollisionManager and so on... (in what file to put Collider? file collider.h)
    // similar goes for DynamicsManager?
    // TODO: optimize all the stupid returns

    CollisionPoints collideSphereSphere(const ColliderSphere* pFirstCollider, const ColliderSphere* pSecondCollider);
    CollisionPoints collideSphereBox(const ColliderSphere* pFirstCollider, const ColliderBox* pSecondCollider);
    CollisionPoints collideSphereMesh(const ColliderSphere* pFirstCollider, const ColliderMesh* pSecondCollider);
    CollisionPoints collideBoxMesh(const ColliderBox* pFirstCollider, const ColliderMesh* pSecondCollider);
    CollisionPoints collideBoxBox(const ColliderBox* pFirstCollider, const ColliderBox* pSecondCollider);
    CollisionPoints collideMeshMesh(const ColliderMesh* pFirstCollider, const ColliderMesh* pSecondCollider);
};


class Collision
{
public:
    Entity* pEntity1;
    Entity* pEntity2;
    CollisionPoints collisionPoints;
};

void log(const Collider& collider);
void log(const ColliderSphere& collider);
void log(const ColliderBox& collider);