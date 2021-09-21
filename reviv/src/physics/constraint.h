#pragma once

#include"scene/entity.h"
#include"collision.h"

class Constraint
{
public:
    enum class ConstraintType { DISTANCE = 0, PENETRATION, FRICTION };

    virtual ~Constraint() = default;

    void solve(float dt);

    ConstraintType type;
    Entity* pFirst = nullptr;
    Entity* pSecond = nullptr;

    virtual ConstraintType getType() const = 0;
    virtual Mat<1,12> getJacobian() const = 0;
    virtual bool getIsBroken() const = 0;
    virtual float getB(float dt) const = 0;
};

class ConstraintDistance : public Constraint
{
public:
    virtual ConstraintType getType() const override;
    virtual Mat<1,12> getJacobian() const override;
    virtual bool getIsBroken() const override;
    virtual float getB(float dt) const override { return 0.f; }

    float distanceSquared;
};

class ConstraintPenetration : public Constraint
{
public:
    virtual ConstraintType getType() const override;
    virtual Mat<1, 12> getJacobian() const override;
    virtual bool getIsBroken() const override;
    virtual float getB(float dt) const override;

    CollisionPoints collisionPoints;
    float bConst = 2.f;
};

class ConstraintFriction : public Constraint
{
public:
    virtual ConstraintType getType() const override;
};
