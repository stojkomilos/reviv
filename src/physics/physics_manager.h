#pragma once

#include"scene/entity.h"
#include"scene/components.h"
#include"core/mat.h"

class PhysicsManager
{
public:
    void updateTransforms();
    void alignPositionAndRotation(const Entity& parentEntity, Entity* childEntity);
    void update();
};
