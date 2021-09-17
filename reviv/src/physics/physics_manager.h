#pragma once

#include"scene/entity.h"
#include"core/mat.h"
#include"scene/scene.h"

#include"physics/collision_manager.h"
#include"physics/dynamics_manager.h"

#include"physics/constraint.h"

class PhysicsManager
{
public:
    ~PhysicsManager() = default;
    PhysicsManager(const PhysicsManager&) = delete;
    PhysicsManager& operator=(const PhysicsManager&) = delete;

    static PhysicsManager* get()
    {
        static PhysicsManager instance;
        return &instance;
    }

    std::vector<Collision> collisions;
    std::vector<Constraint*> constraints;

    Collider* getCollidableFromEntity(Entity* pEntity);

    void init();
    void onUpdate(float dt);
private:
    PhysicsManager() = default;

    void onUpdateDetectCollisions(float dt);
    void onUpdateDynamics(float dt);
    void detectCollisionsNarrowPhase(float dt);
    void onUpdateResolveCollisions(float dt);

    void alignPositionAndRotation(const Entity& parentEntity, Entity* childEntity);

    void calculateNewVelocitiesAndForces(float dt);
    void iterateConstrainst(float dt);
    void calculateNewPositionsAndVelocities(float dt);
};

class PhysicalDynamic
{
public:
    PhysicalDynamic();

    float mass;
    Vec3f velocity;
    Vec3f force;
    Vec3f torque;

    float gravity;
};

void log(const PhysicalDynamic& physical);