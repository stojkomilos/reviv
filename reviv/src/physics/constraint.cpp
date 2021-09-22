#include"constraint.h"

#include"core/mat.h"
#include"scene/components.h"

#include"core/input.h"

#define RV_INFINITE_MASS 100000.f // used for unmovable/unrotatable objects

float ConstraintPenetration::getB(float dt) const
{
    PhysicalDynamic* pFirstPhysical = &pFirst->get<PhysicalComponent>()->physical;
    PhysicalDynamic* pSecondPhysical = &pSecond->get<PhysicalComponent>()->physical;

    Vec3 wra = pFirstPhysical->velocity + cross(pFirstPhysical->angularVelocity, collisionPoints.firstPoint - pFirst->get<TransformComponent>()->getPosition());// TODO: check this
    Vec3 wrb = -pSecondPhysical->velocity - cross(pSecondPhysical->angularVelocity, collisionPoints.secondPoint - pSecond->get<TransformComponent>()->getPosition());// TODO: check this
    float bouncyPart = restitution * dot(-collisionPoints.normal, -pFirstPhysical->velocity - wra + pSecondPhysical->velocity + wrb); // TODO: check this

    return -beta * collisionPoints.depth / dt;
}

float ConstraintDistance::getB(float dt) const
{
    return beta / dt * getConstraintValue();
}

Mat<1,12> ConstraintDistance::getJacobian() const
{

    Vec3 n = pFirst->get<TransformComponent>()->getPosition() - pSecond->get<TransformComponent>()->getPosition();
    n = n / module(n);

    Mat<1,12> jacobian;
    jacobian.fill(0.f);

    *jacobian.getPtr(0, 0) = n.get(0, 0);
    *jacobian.getPtr(0, 1) = n.get(1, 0);
    *jacobian.getPtr(0, 2) = n.get(2, 0);

    *jacobian.getPtr(0, 6) = -n.get(0, 0);
    *jacobian.getPtr(0, 7) = -n.get(1, 0);
    *jacobian.getPtr(0, 8) = -n.get(2, 0);

    return jacobian;
}

Mat<1,12> ConstraintPenetration::getJacobian() const
{
    Vec3 n = collisionPoints.normal;
    n = n / module(n);

    Vec3 r1n = cross(collisionPoints.firstPoint - pFirst->get<TransformComponent>()->getPosition(), n);
    Vec3 r2n = cross(collisionPoints.secondPoint - pSecond->get<TransformComponent>()->getPosition(), n);

    Mat<1,12> jacobian;

    *jacobian.getPtr(0, 0) = -n.get(0, 0);
    *jacobian.getPtr(0, 1) = -n.get(1, 0);
    *jacobian.getPtr(0, 2) = -n.get(2, 0);

    *jacobian.getPtr(0, 3) = -r1n.get(0, 0);
    *jacobian.getPtr(0, 4) = -r1n.get(1, 0);
    *jacobian.getPtr(0, 5) = -r1n.get(2, 0);

    *jacobian.getPtr(0, 6) = n.get(0, 0);
    *jacobian.getPtr(0, 7) = n.get(1, 0);
    *jacobian.getPtr(0, 8) = n.get(2, 0);

    *jacobian.getPtr(0, 9) = r2n.get(0, 0);
    *jacobian.getPtr(0, 10) = r2n.get(1, 0);
    *jacobian.getPtr(0, 11) = r2n.get(2, 0);

    return jacobian;
}

float ConstraintDistance::getConstraintValue() const
{
    Vec3 positionDifference = pFirst->get<TransformComponent>()->getPosition() - pSecond->get<TransformComponent>()->getPosition();
    return dot(positionDifference, positionDifference) - distanceSquared;
}

bool ConstraintDistance::getIsBroken() const
{
    if(abs(getConstraintValue()) > 0.001f)
        return true;
    else return false;
}

bool ConstraintPenetration::getIsBroken() const
{
    return true;
}

void Constraint::solve(float dt)
{

    Mat<12, 1> positions;
    *positions.getPtr(0, 0) = pFirst->get<TransformComponent>()->getPosition().get(0, 0);
    *positions.getPtr(1, 0) = pFirst->get<TransformComponent>()->getPosition().get(1, 0);
    *positions.getPtr(2, 0) = pFirst->get<TransformComponent>()->getPosition().get(2, 0);
    *positions.getPtr(3, 0) = pFirst->get<TransformComponent>()->getRotation().get(0, 0);
    *positions.getPtr(4, 0) = pFirst->get<TransformComponent>()->getRotation().get(1, 0);
    *positions.getPtr(5, 0) = pFirst->get<TransformComponent>()->getRotation().get(2, 0);

    *positions.getPtr(6, 0) = pSecond->get<TransformComponent>()->getPosition().get(0, 0);
    *positions.getPtr(7, 0) = pSecond->get<TransformComponent>()->getPosition().get(1, 0);
    *positions.getPtr(8, 0) = pSecond->get<TransformComponent>()->getPosition().get(2, 0);
    *positions.getPtr(9, 0) = pSecond->get<TransformComponent>()->getRotation().get(0, 0);
    *positions.getPtr(10, 0) = pSecond->get<TransformComponent>()->getRotation().get(1, 0);
    *positions.getPtr(11, 0) = pSecond->get<TransformComponent>()->getRotation().get(2, 0);

    // set velocities

    Mat<12, 1> velocities;

    *velocities.getPtr(0, 0) = pFirst->get<PhysicalComponent>()->physical.velocity.get(0, 0);
    *velocities.getPtr(1, 0) = pFirst->get<PhysicalComponent>()->physical.velocity.get(1, 0);
    *velocities.getPtr(2, 0) = pFirst->get<PhysicalComponent>()->physical.velocity.get(2, 0);
    *velocities.getPtr(3, 0) = pFirst->get<PhysicalComponent>()->physical.angularVelocity.get(0, 0);
    *velocities.getPtr(4, 0) = pFirst->get<PhysicalComponent>()->physical.angularVelocity.get(1, 0);
    *velocities.getPtr(5, 0) = pFirst->get<PhysicalComponent>()->physical.angularVelocity.get(2, 0);

    *velocities.getPtr(6, 0) = pSecond->get<PhysicalComponent>()->physical.velocity.get(0, 0);
    *velocities.getPtr(7, 0) = pSecond->get<PhysicalComponent>()->physical.velocity.get(1, 0);
    *velocities.getPtr(8, 0) = pSecond->get<PhysicalComponent>()->physical.velocity.get(2, 0);
    *velocities.getPtr(9, 0) = pSecond->get<PhysicalComponent>()->physical.angularVelocity.get(0, 0);
    *velocities.getPtr(10, 0) = pSecond->get<PhysicalComponent>()->physical.angularVelocity.get(1, 0);
    *velocities.getPtr(11, 0) = pSecond->get<PhysicalComponent>()->physical.angularVelocity.get(2, 0);

    // set masses and moments of inertia
    
    Vec3 firstPosition = pFirst->get<TransformComponent>()->getPosition();
    Vec3 secondPosition = pSecond->get<TransformComponent>()->getPosition();

    float firstInverseMass = pFirst->get<PhysicalComponent>()->physical.getInverseMass();
    float secondInverseMass = pSecond->get<PhysicalComponent>()->physical.getInverseMass();

    int nrConstraintIterations = 1;

    for(int constraintIteration=0; constraintIteration < nrConstraintIterations; constraintIteration++)
    {
        if(this->getIsBroken())
        {
            Mat<1,12> jacobian = this->getJacobian();

            Mat<12, 12> massInverse;
            massInverse.fill(0.f);

            for(int i=0; i<3; i++) // set mass for linear momentum
            {
                if(pFirst->get<PhysicalComponent>()->physical.fixedTranslation)
                    *massInverse.getPtr(i, i) = 1.f/RV_INFINITE_MASS;
                else *massInverse.getPtr(i, i) = firstInverseMass;

                if(pSecond->get<PhysicalComponent>()->physical.fixedTranslation)
                    *massInverse.getPtr(i+6, i+6) = 1.f/RV_INFINITE_MASS;
                else *massInverse.getPtr(i+6, i+6) = secondInverseMass;
            }

            for(int i=0; i<3; i++)      // set inertial mass
                for(int j=0; j<3; j++)
                {
                    if(pFirst->get<PhysicalComponent>()->physical.fixedRotation)
                        if(i == j)
                            *massInverse.getPtr(i+3, j+3) = 1.f/RV_INFINITE_MASS;
                        else *massInverse.getPtr(i+3, j+3) = 0.f;
                    else *massInverse.getPtr(i+3, j+3) = pFirst->get<PhysicalComponent>()->physical.getInverseInertiaTensor()->get(i, j);

                    if(pSecond->get<PhysicalComponent>()->physical.fixedRotation)
                        if(i == j)
                            *massInverse.getPtr(i+9, j+9) = 1.f/RV_INFINITE_MASS;
                        else *massInverse.getPtr(i+9, j+9) = 0.f;
                    else *massInverse.getPtr(i+9, j+9) = pSecond->get<PhysicalComponent>()->physical.getInverseInertiaTensor()->get(i, j);
                }

            Mat<12, 1> fe;

            *fe.getPtr(0, 0)  = pFirst->get<PhysicalComponent>()->physical.force.get(0, 0);
            *fe.getPtr(1, 0)  = pFirst->get<PhysicalComponent>()->physical.force.get(1, 0);
            *fe.getPtr(2, 0)  = pFirst->get<PhysicalComponent>()->physical.force.get(2, 0);
            *fe.getPtr(3, 0)  = pFirst->get<PhysicalComponent>()->physical.torque.get(0, 0);
            *fe.getPtr(4, 0)  = pFirst->get<PhysicalComponent>()->physical.torque.get(1, 0);
            *fe.getPtr(5, 0)  = pFirst->get<PhysicalComponent>()->physical.torque.get(2, 0);

            *fe.getPtr(6, 0)  = pSecond->get<PhysicalComponent>()->physical.force.get(0, 0);
            *fe.getPtr(7, 0)  = pSecond->get<PhysicalComponent>()->physical.force.get(1, 0);
            *fe.getPtr(8, 0)  = pSecond->get<PhysicalComponent>()->physical.force.get(2, 0);
            *fe.getPtr(9, 0)  = pSecond->get<PhysicalComponent>()->physical.torque.get(0, 0);
            *fe.getPtr(10, 0) = pSecond->get<PhysicalComponent>()->physical.torque.get(1, 0);
            *fe.getPtr(11, 0) = pSecond->get<PhysicalComponent>()->physical.torque.get(2, 0);


            float b = this->getB(dt);
            auto v1 = velocities + massInverse * fe * dt;

            auto left = jacobian * massInverse * transpose(jacobian);
            auto right = (-b - (jacobian * v1).get(0, 0)) / dt;

            Mat<left.height, 1> lambda;
            lambda.randomize(-1, 1);
            solveGaussSeidel(&lambda, left, Vec1(right), 5);

            Mat<12, 1> fc = transpose(jacobian) * lambda;

            if(0) // constraint solver debug
            {
                cout << "jacobian: " << endl;
                log(jacobian);

                cout << "fe: " << endl;
                log(fe);

                cout << "massInverse: " << endl;
                log(massInverse);

                cout << "lambda: " << endl;
                log(lambda);

                cout << "fc: " << endl;
                log(fc);

                cout << "b: " << b << endl;

                cout << "------\n";
            }

            if(pFirst->get<PhysicalComponent>()->physical.fixedTranslation == false)
            {
                *pFirst->get<PhysicalComponent>()->physical.force.getPtr(0, 0) +=  fc.get(0,  0);
                *pFirst->get<PhysicalComponent>()->physical.force.getPtr(1, 0) +=  fc.get(1,  0);
                *pFirst->get<PhysicalComponent>()->physical.force.getPtr(2, 0) +=  fc.get(2,  0);
            }
            if(pFirst->get<PhysicalComponent>()->physical.fixedRotation == false)
            {
                *pFirst->get<PhysicalComponent>()->physical.torque.getPtr(0, 0) += fc.get(3,  0);
                *pFirst->get<PhysicalComponent>()->physical.torque.getPtr(1, 0) += fc.get(4,  0);
                *pFirst->get<PhysicalComponent>()->physical.torque.getPtr(2, 0) += fc.get(5,  0);
            }

            if(pSecond->get<PhysicalComponent>()->physical.fixedTranslation == false)
            {
                *pSecond->get<PhysicalComponent>()->physical.force.getPtr(0, 0) += fc.get(6,  0);
                *pSecond->get<PhysicalComponent>()->physical.force.getPtr(1, 0) += fc.get(7,  0);
                *pSecond->get<PhysicalComponent>()->physical.force.getPtr(2, 0) += fc.get(8,  0);
            }

            if(pSecond->get<PhysicalComponent>()->physical.fixedRotation == false)
            {
                *pSecond->get<PhysicalComponent>()->physical.torque.getPtr(0, 0) += fc.get(9,  0);
                *pSecond->get<PhysicalComponent>()->physical.torque.getPtr(1, 0) += fc.get(10, 0);
                *pSecond->get<PhysicalComponent>()->physical.torque.getPtr(2, 0) += fc.get(11, 0);
            }
        }
    }
}

Constraint::ConstraintType ConstraintDistance::getType() const
{
    return ConstraintType::DISTANCE;
}

Constraint::ConstraintType ConstraintPenetration::getType() const
{
    return ConstraintType::PENETRATION;
}

Constraint::ConstraintType ConstraintFriction::getType() const
{
    return ConstraintType::FRICTION;
}