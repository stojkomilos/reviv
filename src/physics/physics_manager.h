#pragma once

#include"core/entity.h"
#include"core/basic_components.h"
#include"core/mat.h"

class PhysicsManager
{
    void updateTransforms();
    void alignPositionAndRotation(Entity& parentEntity, Entity& childEntity);
    void update()
}
