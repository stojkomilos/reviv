#pragma once

#include"scene/entity.h"
#include"scene/components.h"
#include"core/mat.h"

class PhysicsManager
{
public:
    ~PhysicsManager() = default;
    PhysicsManager(const PhysicsManager&) = delete;
    PhysicsManager& operator=(const PhysicsManager&) = delete;

    static void init() { getInstance()->iInit(); }
    static void onUpdate() { getInstance()->iOnUpdate(); }

    static PhysicsManager* getInstance()
    {
        static PhysicsManager instance;
        return &instance;
    }

private:
    PhysicsManager() = default;
    void iInit();
    void iOnUpdate();

    void alignPositionAndRotation(const Entity& parentEntity, Entity* childEntity);
};