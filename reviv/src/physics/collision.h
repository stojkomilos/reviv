#pragma once

#include"core/mat.h"
#include"scene/entity.h"
#include"renderer/mesh.h"

class TransformComponent;

class ColliderSphere;
class ColliderBox;
class ColliderMesh;

class CollisionPoints
{
public:
    Vec3 firstPoint;
    Vec3 secondPoint;
    Vec3 normal;
    float depth;
    bool hasCollided;
};

class TransformComponent;

class Collider
{
public:
    virtual ~Collider() = default;

    virtual CollisionPoints collide(const Collider* pSecondCollider, const TransformComponent* pFirstTransform, const TransformComponent* pSecondTransform) const = 0;
    virtual Vec3 findFurthestPoint(const Vec3& direction, const TransformComponent* pTransform) const = 0; // a.k.a. support function. used for gjk collision detection

    virtual CollisionPoints collideSphere(const ColliderSphere* pFirstCollider, const TransformComponent* pFirstTransform, 
        const TransformComponent* pSecondTransform) const = 0;

    virtual CollisionPoints collideBox(const ColliderBox* pFirstCollider, const TransformComponent* pFirstTransform, 
        const TransformComponent* pSecondTransform) const = 0;

    virtual CollisionPoints collideMesh(const ColliderMesh* pFirstCollider, const TransformComponent* pFirstTransform, 
        const TransformComponent* pSecondTransform) const = 0;

    //inline TransformComponent* getTransformComponentPtr() { return pTransformComponent; }
    //inline TransformComponent& getTransformComponent() const { return *pTransformComponent; }

private:
    //TransformComponent* pTransformComponent = nullptr;

protected:
    Collider() = default;
};

class ColliderSphere : public Collider
{
public:
    ColliderSphere() = default;

    virtual Vec3 findFurthestPoint(const Vec3& direction, const TransformComponent* pTransform) const override;

    virtual CollisionPoints collide(const Collider* pSecondCollider, const TransformComponent* pFirstTransform, 
        const TransformComponent* pOtherTransform) const override;

    virtual CollisionPoints collideSphere(const ColliderSphere* pFirstCollider, const TransformComponent* pFirstTransform, 
        const TransformComponent* pSecondTransform) const override;

    virtual CollisionPoints collideBox(const ColliderBox* pFirstCollider, const TransformComponent* pFirstTransform, 
        const TransformComponent* pSecondTransform) const override;

    virtual CollisionPoints collideMesh(const ColliderMesh* pFirstCollider, const TransformComponent* pFirstTransform, 
        const TransformComponent* pSecondTransform) const override;

    //float relativeRadius; // the radius gets scaled by the entitys respective transform
};

class ColliderBox : public Collider
{
public:
    virtual Vec3 findFurthestPoint(const Vec3& direction, const TransformComponent* pTransform) const override;

    virtual CollisionPoints collide(const Collider* pSecondCollider, const TransformComponent* pFirstTransform, 
        const TransformComponent* pSecondTransform) const override;

    virtual CollisionPoints collideSphere(const ColliderSphere* pFirstCollider, const TransformComponent* pFirstTransform, 
        const TransformComponent* pSecondTransform) const override;

    virtual CollisionPoints collideBox(const ColliderBox* pFirstCollider, const TransformComponent* pFirstTransform, 
        const TransformComponent* pSecondTransform) const override;

    virtual CollisionPoints collideMesh(const ColliderMesh* pFirstCollider, const TransformComponent* pFirstTransform, 
        const TransformComponent* pSecondTransform) const override;
};

class ColliderMesh : public Collider
{
public:
    virtual Vec3 findFurthestPoint(const Vec3& direction, const TransformComponent* pTransform) const override;

    Mesh* pMesh = nullptr;

    virtual CollisionPoints collide(const Collider* pSecondCollider, const TransformComponent* pFirstTransform, 
        const TransformComponent* pSecondTransform) const override;

    virtual CollisionPoints collideSphere(const ColliderSphere* pFirstCollider, const TransformComponent* pFirstTransform, 
        const TransformComponent* pSecondTransform) const override;

    virtual CollisionPoints collideBox(const ColliderBox* pFirstCollider, const TransformComponent* pFirstTransform, 
        const TransformComponent* pSecondTransform) const override;

    virtual CollisionPoints collideMesh(const ColliderMesh* pFirstCollider, const TransformComponent* pFirstTransform, 
        const TransformComponent* pSecondTransform) const override;
};

class CollisionManager
{
public:
    static CollisionManager* get()
    {
        static CollisionManager instance;
        return &instance;
    }

    void onUpdateDetectCollisions(float dt);
    void onUpdateResolveCollisions(float dt);
    //void detectCollisionsBroadPhase(float dt);
    void detectCollisionsNarrowPhase(float dt);

    //TODO: maybe? place all this in Collider class, remove CollisionManager and so on... (in what file to put Collider? file collider.h)
    // similar goes for DynamicsManager?
    // TODO: optimize all the stupid returns

    static CollisionPoints collideSphereSphere(const ColliderSphere* pFirstCollider, const TransformComponent* pFirstTransform, 
        const ColliderSphere* pSecondCollider, const TransformComponent* pSecondTransform);

    static CollisionPoints collideSphereBox(const ColliderSphere* pFirstCollider, const TransformComponent* pFirstTransform, 
        const ColliderBox* pSecondCollider, const TransformComponent* pSecondTransform);

    static CollisionPoints collideSphereMesh(const ColliderSphere* pFirstCollider, const TransformComponent* pFirstTransform, 
        const ColliderMesh* pSecondCollider, const TransformComponent* pSecondTransform);

    static CollisionPoints collideBoxMesh(const ColliderBox* pFirstCollider, const TransformComponent* pFirstTransform, 
        const ColliderMesh* pSecondCollider, const TransformComponent* pSecondTransform);

    static CollisionPoints collideBoxBox(const ColliderBox* pFirstCollider, const TransformComponent* pFirstTransform, 
        const ColliderBox* pSecondCollider, const TransformComponent* pSecondTransform);

    static CollisionPoints collideMeshMesh(const ColliderMesh* pFirstCollider, const TransformComponent* pFirstTransform, 
        const ColliderMesh* pSecondCollider, const TransformComponent* pSecondTransform);
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