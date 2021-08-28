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

    static void init() { get()->iInit(); }
    static void onUpdate(float dt) { get()->iOnUpdate(dt); }

    static PhysicsManager* get()
    {
        static PhysicsManager instance;
        return &instance;
    }

    std::vector<Collision> collisions;

    Collider* getCollidableFromEntity(Entity* pEntity);
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
