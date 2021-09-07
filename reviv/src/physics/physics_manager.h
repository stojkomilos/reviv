#pragma once

#include"scene/entity.h"
#include"core/mat.h"
#include"scene/scene.h"

#include"physics/collision_manager.h"
#include"physics/dynamics_manager.h"

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
};