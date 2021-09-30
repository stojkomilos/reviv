#pragma once

#include"collision.h"

enum class RigidbodyShape { BOX = 0, SPHERE, BALL };

class Rigidbody
{
public:
    Rigidbody() = delete;
    Rigidbody(const RigidbodyShape& shape, TransformComponent* const pTransformComponent);

    inline float getMass() const { return mass; } // needs a getter and setter for automaticaly chaning inverseMass and inertiaTensor and inverseInertiaTensor
    inline float getInverseMass() const { return inverseMass; }
    inline const Mat<3,3>& getInverseInertiaTensor() const { return inverseInertiaTensor; }
    inline const Mat<3,3>& getInertiaTensor() const { return inertiaTensor; }

    float getKineticEnergy() const;
    float getPotentialEnergy() const;
    float getTotalEnergy() const; // returns kinteic + potential energy

    void setMass(float newMass);

    inline const RigidbodyShape& getShape() const { return shape; } // unrelated to the collider shape
    void setShape(RigidbodyShape newShape); // unrelated to the collider shape

    inline Collider* getCollider() const { RV_ASSERT(pCollider != nullptr, "collider not set"); return pCollider; }
    ColliderSphere* setColliderSphere();
    ColliderMesh* setColliderMesh(const Mesh* pMesh);
    
    inline TransformComponent* getTransform() { RV_ASSERT(pTransformComponent != nullptr, ""); return pTransformComponent; }
    inline const TransformComponent* getTransform() const { RV_ASSERT(pTransformComponent != nullptr, ""); return pTransformComponent; }

    void calculateMomentOfInertia();

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

    RigidbodyShape shape;

    Collider* pCollider = nullptr;
    TransformComponent* const pTransformComponent;
};

void log(const Rigidbody& physical);