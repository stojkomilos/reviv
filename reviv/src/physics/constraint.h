#pragma once

#include"collision.h"

class Entity;

class Constraint
{
public:
    enum class ConstraintType { DISTANCE = 0, PENETRATION, FRICTION };

    virtual ~Constraint() = default;

    void solve(float dt);

    virtual ConstraintType getType() const = 0;
    virtual Mat<1,12> getJacobian() const = 0;
    virtual bool getIsBroken() const = 0;
    virtual float getConstraintValue() const = 0;
    virtual float getB(float dt) const = 0;

    ConstraintType type;
    Entity* pFirst = nullptr;
    Entity* pSecond = nullptr;

    float beta = 0.8f;
};

class ConstraintDistance : public Constraint
{
public:
    virtual ConstraintType getType() const override;
    virtual Mat<1,12> getJacobian() const override;
    virtual float getConstraintValue() const override;
    virtual bool getIsBroken() const override;
    virtual float getB(float dt) const override;

    float distanceSquared;
};

class ConstraintPenetration : public Constraint
{
public:
    virtual ConstraintType getType() const override;
    virtual Mat<1, 12> getJacobian() const override;
    virtual float getConstraintValue() const override { RV_ASSERT(false, ""); return 0.f;  }
    virtual bool getIsBroken() const override;
    virtual float getB(float dt) const override;

    CollisionPoints collisionPoints;
    float restitution;
};

class ConstraintFriction : public Constraint
{
public:
    virtual ConstraintType getType() const override;
};
