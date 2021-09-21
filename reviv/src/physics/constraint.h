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
    virtual void getJacobian(Mat<1, 12>* pJacobian) const = 0;
    virtual bool getIsBroken() const = 0;
};

class ConstraintDistance : public Constraint
{
public:
    virtual ConstraintType getType() const override;
    virtual void getJacobian(Mat<1, 12>* pJacobian) const override;
    virtual bool getIsBroken() const override;

    float distanceSquared;
};

class ConstraintPenetration : public Constraint
{
public:
    virtual ConstraintType getType() const override;
    virtual void getJacobian(Mat<1, 12>* pJacobian) const override;
    virtual bool getIsBroken() const override;

    CollisionPoints collisionPoints;
};

class ConstraintFriction : public Constraint
{
public:
    virtual ConstraintType getType() const override;
};
