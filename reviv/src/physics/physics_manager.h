#pragma once

#include"scene/entity.h"
#include"core/mat.h"
#include"scene/scene.h"

#include"physics/collision.h"

#include"physics/constraint.h"

// semi-implicit euler general rule: to keep it stable, have 4 updates per oscillation
//                              

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

    std::vector<ConstraintPenetration> constraintsCollision;
    std::vector<Constraint*> constraintsGeneral;

    void init();
    void onUpdate(float dt);

    void debugDrawingCollisions(); // Debug for collisions

private:
    PhysicsManager() = default;

    void satisfyConstraintsCollision(float dt);

    void alignPositionAndRotation(const Entity& parentEntity, Entity* childEntity);

    void precalculateVelocitiesAndForces(float dt);
    void satisfyConstraintsGeneral(float dt);
    void calculateNewPositionsVelocitiesAndForces(float dt);

    void doCollisionDetection(float dt);
    void doCollisionDetectionNarrowPhase(float dt);
};