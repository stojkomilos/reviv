#pragma once

#include"scene/entity.h"
#include"core/mat.h"
#include"scene/scene.h"

#include"physics/collision.h"

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

    std::vector<ConstraintPenetration> constraintsCollision;
    std::vector<Constraint*> constraintsGeneral;

    Collider* getCollidableFromEntity(Entity* pEntity);

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

class PhysicalDynamic
{
public:
    PhysicalDynamic(float mass = 1.f);

    inline float getMass() const { return mass; } // needs a getter and setter for automaticaly chaning inverseMass and inertiaTensor and inverseInertiaTensor
    inline float getInverseMass() const { return inverseMass; }
    inline const Mat<3,3>* getInverseInertiaTensor() const { return &inverseInertiaTensor; }

    float getKineticEnergy();

    void setMass(float newMass);
    
    Vec3 velocity;
    Vec3 angularVelocity;
    Vec3 force;
    Vec3 torque;

    float gravity;
    float restitution = 0.8f;

    bool fixedTranslation = false;
    bool fixedRotation = false;

private:
    float mass;
    float inverseMass;
    Mat<3,3> inertiaTensor;
    Mat<3,3> inverseInertiaTensor;
};

void log(const PhysicalDynamic& physical);