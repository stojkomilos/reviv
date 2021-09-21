#include"constraint.h"

#include"core/mat.h"
#include"scene/components.h"

#include"core/input.h"

#define RV_INFINITE_MASS 100000000.f // used for unmovable/unrotatable objects

void Constraint::solve(float dt)
{

    Mat<12, 1> positions;
    *positions.getPtr(0, 0) = pFirst->get<TransformComponent>()->position.get(0, 0);
    *positions.getPtr(1, 0) = pFirst->get<TransformComponent>()->position.get(1, 0);
    *positions.getPtr(2, 0) = pFirst->get<TransformComponent>()->position.get(2, 0);
    *positions.getPtr(3, 0) = pFirst->get<TransformComponent>()->rotation.get(0, 0);
    *positions.getPtr(4, 0) = pFirst->get<TransformComponent>()->rotation.get(1, 0);
    *positions.getPtr(5, 0) = pFirst->get<TransformComponent>()->rotation.get(2, 0);

    *positions.getPtr(6, 0) = pSecond->get<TransformComponent>()->position.get(0, 0);
    *positions.getPtr(7, 0) = pSecond->get<TransformComponent>()->position.get(1, 0);
    *positions.getPtr(8, 0) = pSecond->get<TransformComponent>()->position.get(2, 0);
    *positions.getPtr(9, 0) = pSecond->get<TransformComponent>()->rotation.get(0, 0);
    *positions.getPtr(10, 0) = pSecond->get<TransformComponent>()->rotation.get(1, 0);
    *positions.getPtr(11, 0) = pSecond->get<TransformComponent>()->rotation.get(2, 0);

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
    *velocities.getPtr(3, 0) = pSecond->get<PhysicalComponent>()->physical.angularVelocity.get(0, 0);
    *velocities.getPtr(4, 0) = pSecond->get<PhysicalComponent>()->physical.angularVelocity.get(1, 0);
    *velocities.getPtr(5, 0) = pSecond->get<PhysicalComponent>()->physical.angularVelocity.get(2, 0);

    // set masses and moments of inertia
    
    Vec3 firstPosition = pFirst->get<TransformComponent>()->position;
    Vec3 secondPosition = pSecond->get<TransformComponent>()->position;

    float firstInverseMass = pFirst->get<PhysicalComponent>()->physical.getInverseMass();
    float secondInverseMass = pSecond->get<PhysicalComponent>()->physical.getInverseMass();

    int nrConstraintIterations = 1;

    for(int constraintIteration=0; constraintIteration < nrConstraintIterations; constraintIteration++)
    {
        if(this->getIsBroken())
        {
            Mat<1, 12> jacobian;

            this->getJacobian(&jacobian);

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
            fe.fill(0.f);

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


            //MatHeap right1(massInverse.height, fe.width); // m^(-1) * F_{ext}
            //multiply(&right1, massInverse, fe);
            Mat<massInverse.getHeight(), fe.getWidth()> right1 = massInverse * fe;

            Mat<velocities.getHeight(), velocities.getWidth()> right2 = velocities * (1.f/dt); // v / dt

            //Mat<right2.getHeight(), right2.getWidth()> right3 = right1 * right2;
            Mat<right2.getHeight(), right2.getWidth()> right3;

            Mat<jacobian.getHeight(), right1.getWidth()> right4 = jacobian * right3;

            Mat<right4.getHeight(), right4.getWidth()> rightTotal = right4 * -1.f;

            Mat<jacobian.getWidth(), jacobian.getHeight()> jacobianTrans = transpose(jacobian);

            Mat<massInverse.getHeight(), jacobianTrans.getWidth()> left1 = massInverse * jacobianTrans;

            Mat<jacobian.getHeight(), left1.getWidth()> leftTotal = jacobian * left1;

            RV_ASSERT(leftTotal.height == leftTotal.width, "can't inverse a non-square matrix");
            Mat<jacobianTrans.getWidth(), 1> lambda;
            lambda.fill(0.f);

            solveGaussSeidel(&lambda, leftTotal, rightTotal, 10);

            Mat<12, 1> fc = jacobianTrans * lambda;

            if(1)
            {
                cout << "jacobian: " << endl;
                log(jacobian);

                cout << "jacobianTrans: " << endl;
                log(jacobianTrans);

                cout << "fe: " << endl;
                log(fe);

                cout << "massInverse: " << endl;
                log(massInverse);

                cout << "righ1: " << endl;
                log(right1);
                cout << "righ2: " << endl;
                log(right2);
                cout << "righTOTAL: " << endl;
                log(rightTotal);
                cout << "left1: " << endl;
                log(left1);
                cout << "leftTOTAL: " << endl;
                log(leftTotal);


                cout << "lambda: " << endl;
                log(lambda);

                cout << "fc: " << endl;
                log(fc);
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

void ConstraintDistance::getJacobian(Mat<1, 12>* pJacobian) const
{

    Vec3 n = pFirst->get<TransformComponent>()->position - pSecond->get<TransformComponent>()->position;
    n = n / module(n);

    *pJacobian->getPtr(0, 0) = n.get(0, 0);
    *pJacobian->getPtr(0, 1) = n.get(1, 0);
    *pJacobian->getPtr(0, 2) = n.get(2, 0);

    *pJacobian->getPtr(0, 6) = -n.get(0, 0);
    *pJacobian->getPtr(0, 7) = -n.get(1, 0);
    *pJacobian->getPtr(0, 8) = -n.get(2, 0);
}

void ConstraintPenetration::getJacobian(Mat<1, 12>* pJacobian) const
{
    Vec3 n = collisionPoints.normal;
    n = n / module(n);

    Vec3 r1n = cross(collisionPoints.firstPoint - pFirst->get<TransformComponent>()->position, n);
    Vec3 r2n = cross(collisionPoints.secondPoint - pSecond->get<TransformComponent>()->position, n);

    *pJacobian->getPtr(0, 0) = -n.get(0, 0);
    *pJacobian->getPtr(0, 1) = -n.get(1, 0);
    *pJacobian->getPtr(0, 2) = -n.get(2, 0);

    *pJacobian->getPtr(0, 3) = -r1n.get(0, 0);
    *pJacobian->getPtr(0, 4) = -r1n.get(1, 0);
    *pJacobian->getPtr(0, 5) = -r1n.get(2, 0);

    *pJacobian->getPtr(0, 6) = n.get(0, 0);
    *pJacobian->getPtr(0, 7) = n.get(1, 0);
    *pJacobian->getPtr(0, 8) = n.get(2, 0);

    *pJacobian->getPtr(0, 9) = r2n.get(0, 0);
    *pJacobian->getPtr(0, 10) = r2n.get(1, 0);
    *pJacobian->getPtr(0, 11) = r2n.get(2, 0);
}

bool ConstraintDistance::getIsBroken() const
{
    Vec3 positionDifference = pFirst->get<TransformComponent>()->position - pSecond->get<TransformComponent>()->position;
    if(abs(dot(positionDifference, positionDifference) - distanceSquared) > 0.001f)
        return true;
    else return false;
}

bool ConstraintPenetration::getIsBroken() const
{
    return true;
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