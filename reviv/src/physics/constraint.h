#pragma once

#include"scene/entity.h"

class Constraint
{
public:
    enum class ConstraintType { DISTANCE = 0, NORMAL, FRICTION };

    virtual ~Constraint() = default;

    void solve(float dt);

    ConstraintType type;
    Entity* pFirst = nullptr;
    Entity* pSecond = nullptr;

    virtual ConstraintType getType() const = 0;
    virtual void getJacobian(MatN* pJacobian) const = 0;
    virtual bool getIsBroken() const = 0;
};

class ConstraintDistance : public Constraint
{
public:
    virtual ConstraintType getType() const override;
    virtual void getJacobian(MatN* pJacobian) const override;
    virtual bool getIsBroken() const override;

    float distanceSquared;
};

class ConstraintNormal : public Constraint
{
public:
    virtual ConstraintType getType() const override;
};

class ConstraintFriction : public Constraint
{
public:
    virtual ConstraintType getType() const override;
};
