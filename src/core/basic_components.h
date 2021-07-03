#pragma once

#include"entity.h"
#include"core/mat.h"

struct TransformComponent : SpecificComponent<TransformComponent>
{
    Mat4 transform;

    TransformComponent() = default; // default constructor
    TransformComponent(const TransformComponent&) = default; // copy constructor
    TransformComponent(const Mat4& transform) // convenient constructor that takes parameters
        : transform(transform) {}
    
    //operator const Mat4& () const { return transform; } // allows implicit (and explicit) cast, idk how it works. Allows stuff like DoMath(TranformComponent asdf)
    operator Mat4& () { return transform; }
};

struct PositionComponent : SpecificComponent<PositionComponent>
{
public:
    Vec3f position;

    PositionComponent() = default;
    PositionComponent(const PositionComponent&) = default;
    PositionComponent(const Vec3f& position)
        : position(position) {}

    //operator const PositionComponent&() const { return position };
    operator Vec3f& () { return position; }
};

struct RotationComponent : SpecificComponent<RotationComponent>
{
    float pitch, yaw, roll;

    RotationComponent() = default;
    RotationComponent(const RotationComponent&) = default;
    RotationComponent(const Vec3f rotation)
        : pitch(rotation.x), yaw(rotation.y), roll(rotation.z) {}

    operator Vec3f () { return Vec3f(pitch, yaw, roll); }
};