#include"constraint.h"

#include"core/mat.h"
#include"scene/components.h"

#include"core/input.h"

void Constraint::solve(float dt)
{

    MatN positions(12, 1);
    *positions.getPtr(0, 0) = pFirst->get<TransformComponent>()->position.a[0];
    *positions.getPtr(1, 0) = pFirst->get<TransformComponent>()->position.a[1];
    *positions.getPtr(2, 0) = pFirst->get<TransformComponent>()->position.a[2];

    *positions.getPtr(3, 0) = 0;
    *positions.getPtr(4, 0) = 0;
    *positions.getPtr(5, 0) = 0;

    *positions.getPtr(6, 0) = pSecond->get<TransformComponent>()->position.a[0];
    *positions.getPtr(7, 0) = pSecond->get<TransformComponent>()->position.a[1];
    *positions.getPtr(8, 0) = pSecond->get<TransformComponent>()->position.a[2];

    *positions.getPtr(9, 0) = 0;
    *positions.getPtr(10, 0) = 0;
    *positions.getPtr(11, 0) = 0;

    // set velocities

    MatN velocities(12, 1);

    *velocities.getPtr(0, 0) = pFirst->get<PhysicalComponent>()->physical.velocity.a[0];
    *velocities.getPtr(1, 0) = pFirst->get<PhysicalComponent>()->physical.velocity.a[1];
    *velocities.getPtr(2, 0) = pFirst->get<PhysicalComponent>()->physical.velocity.a[2];

    *velocities.getPtr(3, 0) = 0;
    *velocities.getPtr(4, 0) = 0;
    *velocities.getPtr(5, 0) = 0;

    *velocities.getPtr(6, 0) = pSecond->get<PhysicalComponent>()->physical.velocity.a[0];
    *velocities.getPtr(7, 0) = pSecond->get<PhysicalComponent>()->physical.velocity.a[1];
    *velocities.getPtr(8, 0) = pSecond->get<PhysicalComponent>()->physical.velocity.a[2];

    *velocities.getPtr(9, 0)  = 0;
    *velocities.getPtr(10, 0) = 0;
    *velocities.getPtr(11, 0) = 0;

    // set masses and moments of inertia
    
    MatN mass(12, 12, 0.f);

    *mass.getPtr(0, 0) = pFirst->get<PhysicalComponent>()->physical.mass;
    *mass.getPtr(1, 1) = pFirst->get<PhysicalComponent>()->physical.mass;
    *mass.getPtr(2, 2) = pFirst->get<PhysicalComponent>()->physical.mass;

    *mass.getPtr(6, 6) = pSecond->get<PhysicalComponent>()->physical.mass;
    *mass.getPtr(7, 7) = pSecond->get<PhysicalComponent>()->physical.mass;
    *mass.getPtr(8, 8) = pSecond->get<PhysicalComponent>()->physical.mass;

    Vec3f firstPosition = pFirst->get<TransformComponent>()->position;
    Vec3f secondPosition = pSecond->get<TransformComponent>()->position;

    int nrConstraintIterations = 1;
    float constraintDistnace = 5.f;

    if(Input::get()->isKeyPressed(RV_KEY_P))
    {
        for(int constraintIteration=0; constraintIteration < nrConstraintIterations; constraintIteration++)
        {
            if(this->getIsBroken())
            {
                cout << "c broken. distance: " << module(firstPosition - secondPosition) << endl;
                MatN jacobian(1, 12, 0.f); // jacobian

                this->getJacobian(&jacobian);

                MatN massInverse(12, 12);
                massInverse.setToIdentity(); // TODO: actually invert this, and cache it

                MatN fe(12, 1, 0.f);

                *fe.getPtr(0, 0)  = pFirst->get<PhysicalComponent>()->physical.force.a[0];
                *fe.getPtr(1, 0)  = pFirst->get<PhysicalComponent>()->physical.force.a[1];
                *fe.getPtr(2, 0)  = pFirst->get<PhysicalComponent>()->physical.force.a[2];
                *fe.getPtr(3, 0)  = pFirst->get<PhysicalComponent>()->physical.torque.a[0];
                *fe.getPtr(4, 0)  = pFirst->get<PhysicalComponent>()->physical.torque.a[1];
                *fe.getPtr(5, 0)  = pFirst->get<PhysicalComponent>()->physical.torque.a[2];

                *fe.getPtr(6, 0)  = pSecond->get<PhysicalComponent>()->physical.force.a[0];
                *fe.getPtr(7, 0)  = pSecond->get<PhysicalComponent>()->physical.force.a[1];
                *fe.getPtr(8, 0)  = pSecond->get<PhysicalComponent>()->physical.force.a[2];
                *fe.getPtr(9, 0)  = pSecond->get<PhysicalComponent>()->physical.torque.a[0];
                *fe.getPtr(10, 0) = pSecond->get<PhysicalComponent>()->physical.torque.a[1];
                *fe.getPtr(11, 0) = pSecond->get<PhysicalComponent>()->physical.torque.a[2];

                MatN right1(massInverse.height, fe.width); // m^(-1) * F_{ext}
                multiply(&right1, massInverse, fe);

                MatN right2(velocities.height, velocities.width); // v / dt
                multiply(&right2, velocities, 1.f/dt);

                MatN right3(right2.height, right2.width);
                add(&right3, right1, right2);

                MatN right4(jacobian.height, right1.width);
                multiply(&right4, jacobian, right3);

                MatN rightTotal(right4.height, right4.width);
                multiply(&rightTotal, right4, -1.f);

                MatN jacobianTrans(jacobian.width, jacobian.height);
                transpose(&jacobianTrans, jacobian);

                MatN left1(massInverse.height, jacobianTrans.width);
                multiply(&left1, massInverse, jacobianTrans);

                MatN leftTotal(jacobian.height, left1.width);
                multiply(&leftTotal, jacobian, left1);

                RV_ASSERT(leftTotal.height == leftTotal.width, "can't inverse a non-square matrix");
                MatN lambda(jacobianTrans.width, 1, 0.f);

                solveGaussSeidel(&lambda, leftTotal, rightTotal, 10);

                MatN fc(12, 1);
                multiply(&fc, jacobianTrans, lambda);

                if(0)
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

                pFirst->get<PhysicalComponent>()->physical.force.a[0] +=  fc.get(0,  0);
                pFirst->get<PhysicalComponent>()->physical.force.a[1] +=  fc.get(1,  0);
                pFirst->get<PhysicalComponent>()->physical.force.a[2] +=  fc.get(2,  0);
                pFirst->get<PhysicalComponent>()->physical.torque.a[0] += fc.get(3,  0);
                pFirst->get<PhysicalComponent>()->physical.torque.a[1] += fc.get(4,  0);
                pFirst->get<PhysicalComponent>()->physical.torque.a[2] += fc.get(5,  0);

                pSecond->get<PhysicalComponent>()->physical.force.a[0] += fc.get(6,  0);
                pSecond->get<PhysicalComponent>()->physical.force.a[1] += fc.get(7,  0);
                pSecond->get<PhysicalComponent>()->physical.force.a[2] += fc.get(8,  0);
                pSecond->get<PhysicalComponent>()->physical.force.a[0] += fc.get(9,  0);
                pSecond->get<PhysicalComponent>()->physical.force.a[1] += fc.get(10, 0);
                pSecond->get<PhysicalComponent>()->physical.force.a[2] += fc.get(11, 0);
            }
            else {
                cout << "c is GOOD " << endl;
                cout << "distance: " << module(firstPosition - secondPosition) << endl;

                cout << "pFirst position: " << endl;
                log(pFirst->get<TransformComponent>()->position);
                cout << "pFirst velocity: " << endl;
                log(pFirst->get<PhysicalComponent>()->physical.velocity);

                cout << "pSecond position: " << endl;
                log(pSecond->get<TransformComponent>()->position);
                cout << "pSecond velocity: " << endl;
                log(pSecond->get<PhysicalComponent>()->physical.velocity);

                cout << "-----\n";
            }
        }
    }

}

void ConstraintDistance::getJacobian(MatN* pJacobian) const
{
    assert(pJacobian->height == 1 && pJacobian->width == 12);

    Vec3f positionDifference = pFirst->get<TransformComponent>()->position - pSecond->get<TransformComponent>()->position;
    float distanceLength = module(positionDifference);

    *pJacobian->getPtr(0, 0) = positionDifference.a[0] / distanceLength;
    *pJacobian->getPtr(0, 1) = positionDifference.a[1] / distanceLength;
    *pJacobian->getPtr(0, 2) = positionDifference.a[2] / distanceLength;

    *pJacobian->getPtr(0, 6) = -positionDifference.a[0] / distanceLength;
    *pJacobian->getPtr(0, 7) = -positionDifference.a[1] / distanceLength;
    *pJacobian->getPtr(0, 8) = -positionDifference.a[2] / distanceLength;
}

Constraint::ConstraintType ConstraintDistance::getType() const
{
    return ConstraintType::DISTANCE;
}

Constraint::ConstraintType ConstraintNormal::getType() const
{
    return ConstraintType::NORMAL;
}

Constraint::ConstraintType ConstraintFriction::getType() const
{
    return ConstraintType::FRICTION;
}

bool ConstraintDistance::getIsBroken() const
{
    Vec3f positionDifference = pFirst->get<TransformComponent>()->position - pSecond->get<TransformComponent>()->position;
    if(abs(dot(positionDifference, positionDifference) - distanceSquared) > 0.001f)
        return true;
    else return false;
}