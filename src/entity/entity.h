#pragma once

#include "core/mat.h"

class Entity{
public:
    Entity();
    mat::vec3 position;
    float pitch, yaw, roll;
    bool valid;
};
