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

class PhysicalDynamic
{
public:
    PhysicalDynamic();

    float mass;
    Vec3f velocity;
    Vec3f force;

    float gravity;
};

void log(const PhysicalDynamic physical);