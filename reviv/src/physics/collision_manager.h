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
    Vec3f firstPoint;
    Vec3f secondPoint;
    Vec3f normal;
    float depth;
    bool hasCollided;
};

class Collider
{
public:
    virtual ~Collider() = default;
    virtual CollisionPoints collide(Collider* pSecondCollider, TransformComponent* pFirstTransform, TransformComponent* pSecondTransform) = 0;

    virtual CollisionPoints collideSphere(ColliderSphere* pFirstCollider, TransformComponent* pFirstTransform, TransformComponent* pSecondTransform) = 0;
    virtual CollisionPoints collideBox(ColliderBox* pFirstCollider, TransformComponent* pFirstTransform, TransformComponent* pSecondTransform) = 0;
    virtual CollisionPoints collideMesh(ColliderMesh* pFirstCollider, TransformComponent* pFirstTransform, TransformComponent* pSecondTransform) = 0;

protected:
    Collider() = default;
};

class ColliderBox : public Collider
{
    virtual CollisionPoints collide(Collider* pSecondCollider, TransformComponent* pFirstTransform, TransformComponent* pSecondTransform) override;
    virtual CollisionPoints collideSphere(ColliderSphere* pFirstCollider, TransformComponent* pFirstTransform, TransformComponent* pSecondTransform) override;
    virtual CollisionPoints collideBox(ColliderBox* pFirstCollider, TransformComponent* pFirstTransform, TransformComponent* pSecondTransform) override;
    virtual CollisionPoints collideMesh(ColliderMesh* pFirstCollider, TransformComponent* pFirstTransform, TransformComponent* pSecondTransform) override;
};

class ColliderMesh : public Collider
{
    Mesh* pMesh;
    virtual CollisionPoints collide(Collider* pSecondCollider, TransformComponent* pFirstTransform, TransformComponent* pSecondTransform) override;
    virtual CollisionPoints collideSphere(ColliderSphere* pFirstCollider, TransformComponent* pFirstTransform, TransformComponent* pSecondTransform) override;
    virtual CollisionPoints collideBox(ColliderBox* pFirstCollider, TransformComponent* pFirstTransform, TransformComponent* pSecondTransform) override;
    virtual CollisionPoints collideMesh(ColliderMesh* pFirstCollider, TransformComponent* pFirstTransform, TransformComponent* pSecondTransform) override;
};

class ColliderSphere : public Collider
{
public:
    ColliderSphere() = default;
    ColliderSphere(float relativeRadius)
        : relativeRadius(relativeRadius) { }

    virtual CollisionPoints collide(Collider* pSecondCollider, TransformComponent* pFirstTransform, TransformComponent* pOtherTransform) override;
    virtual CollisionPoints collideSphere(ColliderSphere* pFirstCollider, TransformComponent* pFirstTransform, TransformComponent* pSecondTransform) override;
    virtual CollisionPoints collideBox(ColliderBox* pFirstCollider, TransformComponent* pFirstTransform, TransformComponent* pSecondTransform) override;
    virtual CollisionPoints collideMesh(ColliderMesh* pFirstCollider, TransformComponent* pFirstTransform, TransformComponent* pSecondTransform) override;

    float relativeRadius; // the radius gets scaled by the entitys respective transform
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
    static CollisionPoints collideSphereSphere(ColliderSphere* pFirstCollider, TransformComponent* pFirstTransform, ColliderSphere* pSecondCollider, TransformComponent* pSecondTransform);
    static CollisionPoints collideSphereBox(ColliderSphere* pFirstCollider, TransformComponent* pFirstTransform, ColliderBox* pSecondCollider, TransformComponent* pSecondTransform);
    static CollisionPoints collideSphereMesh(ColliderSphere* pFirstCollider, TransformComponent* pFirstTransform, ColliderMesh* pSecondCollider, TransformComponent* pSecondTransform);
    static CollisionPoints collideBoxMesh(ColliderBox* pFirstCollider, TransformComponent* pFirstTransform, ColliderMesh* pSecondCollider, TransformComponent* pSecondTransform);
    static CollisionPoints collideBoxBox(ColliderBox* pFirstCollider, TransformComponent* pFirstTransform, ColliderBox* pSecondCollider, TransformComponent* pSecondTransform);
    static CollisionPoints collideMeshMesh(ColliderMesh* pFirstCollider, TransformComponent* pFirstTransform, ColliderMesh* pSecondCollider, TransformComponent* pSecondTransform);
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