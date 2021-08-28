#pragma once

#include"core/mat.h"

class DynamicsManager
{
public:
    static DynamicsManager* get()
    {
        static DynamicsManager instance;
        return &instance;
    }
private:
    DynamicsManager() = default;
    void onUpdate(float dt);
    friend class PhysicsManager;
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

void log(const PhysicalDynamic physical);