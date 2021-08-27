#pragma once

#include"scene/entity.h"
//#include"scene/components.h"
#include"core/mat.h"
#include"scene/scene.h"
//#include"scene/components.h"

class PhysicsManager
{
public:
    ~PhysicsManager() = default;
    PhysicsManager(const PhysicsManager&) = delete;
    PhysicsManager& operator=(const PhysicsManager&) = delete;

    static void init() { getInstance()->iInit(); }
    static void onUpdate(float dt) { getInstance()->iOnUpdate(dt); }

    static PhysicsManager* getInstance()
    {
        static PhysicsManager instance;
        return &instance;
    }


private:
    PhysicsManager() = default;
    void iInit();
    void iOnUpdate(float dt);

    void onUpdateDetectCollisions(float dt);
    void onUpdateDynamics(float dt);
    void detectCollisionsNarrowPhase(float dt);
    void onUpdateResolveCollisions(float dt);

    void alignPositionAndRotation(const Entity& parentEntity, Entity* childEntity);
};

class Collider
{
protected:
    Collider() = default;
};

class ColliderSphere : public Collider
{
public:
    float radius;
};

class ColliderAabb
{
public:
    float relativeXMinus, relativeXPlus;
    float relativeYMinus, relativeYPlus;
};

class TransformComponent;
class PhysicalStatic // separated into ObjectStatic and ObjectDynamic to save memory for static terrain
{
public:
    //TransformComponent* pTransform = nullptr;

protected:
    //PhysicalStatic(TransformComponent* pTransformComponent);
    PhysicalStatic() = default; // temporary, later can be made public
};

class PhysicalDynamic : public PhysicalStatic
{
public:
    PhysicalDynamic();

    float mass;
    Vec3f velocity;
    Vec3f force;

    float gravity;
};

class CollisionPoints
{
    Vec3f a;
    Vec3f b;
    Vec3f normal;
    float depth;
    bool hasCollided;
};

void log(const PhysicalDynamic& physical);
void log(const ColliderSphere& collider);
void log(const ColliderAabb& collider);