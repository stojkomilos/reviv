#pragma once

#include"core/entity.h"
#include"core/basic_components.h"
#include"core/mat.h"

class PhysicsManager
{
public:
    void updateTransforms();
    void alignPositionAndRotation(const Entity& parentEntity, Entity* childEntity);
    void update();
};
