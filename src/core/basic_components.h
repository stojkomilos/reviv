#pragma once

#include"entity.h"
#include"core/mat.h"

class Transform : public SpecificComponent<Transform>
{
public:
    Mat4 transform;

    Transform() = default; // default constructor
    Transform(const Transform&) = default; // copy constructor
    Transform(const Mat4& transform) // convenient constructor that takes parameters
        : transform(transform) {}
    
    //operator const Mat4& () const { return transform; } // allows implicit (and explicit) cast, idk how it works. Allows stuff like DoMath(TranformComponent asdf)
    operator Mat4& () { return transform; }
};

class PositionComponent : public SpecificComponent<PositionComponent>
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

class RotationComponent : public SpecificComponent<RotationComponent>
{
public:
    float pitch, yaw, roll;

    RotationComponent() = default;
    RotationComponent(const RotationComponent&) = default;
    RotationComponent(const Vec3f rotation)
        : pitch(rotation.x), yaw(rotation.y), roll(rotation.z) {}

    operator Vec3f () { return Vec3f(pitch, yaw, roll); }
};