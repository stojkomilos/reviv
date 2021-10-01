#include"rigidbody.h"

#include"scene/scene.h"
#include"scene/components.h"

Rigidbody::Rigidbody(const RigidbodyShape& shape, TransformComponent* const pTransformComponent)
    : velocity{0, 0, 0}, angularVelocity{0, 0, 0}, force{0, 0, 0}, torque{0, 0, 0}, pTransformComponent(pTransformComponent), shape(shape)
{
    gravity = Scene::getGravity();
    setMass(1.f);
}

void Rigidbody::setMass(float newMass)
{
    mass = newMass;
    inverseMass = 1.f / newMass;

    calculateMomentOfInertia();
}

float Rigidbody::getKineticEnergy() const
{
    float linearEnergy = mass / 2.f * dot(velocity, velocity);

    float angularEnergy = 0.f;
    for(int i=0; i<3; i++)
    {
        angularEnergy += inertiaTensor.get(i, 0) / 2.f * angularVelocity.get(i, 0) * angularVelocity.get(i, 0);
    }
        
    return linearEnergy + angularEnergy;
}

float Rigidbody::getPotentialEnergy() const
{
    return gravity * mass * getTransform()->getPosition().get(2, 0);
}

float Rigidbody::getTotalEnergy() const
{
    return getKineticEnergy() + getPotentialEnergy();
}

void Rigidbody::calculateMomentOfInertia()
{
    switch(shape)
    {
        case RigidbodyShape::SPHERE:        // a.k.a. hollow sphere
        {
            float radiusSquared = getTransform()->getScale().get(0, 0) /2.f * getTransform()->getScale().get(0, 0)/2.f;
            for(int i=0; i<3; i++)
                for(int j=0; j<3; j++)
                    if(i == j)
                        *inertiaTensor.getPtr(i, j) = 2.f/3.f * mass * radiusSquared;
                    else *inertiaTensor.getPtr(i, j) = 0.f;
            break;
        }

        case RigidbodyShape::BALL:  // a.k.a. sollid sphere
        {
            float radiusSquared = getTransform()->getScale().get(0, 0)/2.f * getTransform()->getScale().get(0, 0)/2.f;
            for(int i=0; i<3; i++)
                for(int j=0; j<3; j++)
                    if(i == j)
                        *inertiaTensor.getPtr(i, j) = 2.f/5.f * mass * radiusSquared;
                    else *inertiaTensor.getPtr(i, j) = 0.f;
            break;
        }
        
        case RigidbodyShape::BOX:
        {
            inertiaTensor.fill(0.f);
            float depth = getTransform()->getScale().get(0, 0);       // if the camera is looking down the positive x axis
            float width = getTransform()->getScale().get(1, 0);
            float height = getTransform()->getScale().get(2, 0);
            *inertiaTensor.getPtr(0, 0) = 1.f/12.f * mass * (width * width + height * height);
            *inertiaTensor.getPtr(1, 1) = 1.f/12.f * mass * (depth * depth + height * height);
            *inertiaTensor.getPtr(2, 2) = 1.f/12.f * mass * (depth * depth + width * width);
            break;
        }

        default:
            RV_ASSERT(false, ""); // should not be in here
    }

    inverseInertiaTensor = inverse(inertiaTensor);
}

ColliderSphere* Rigidbody::setColliderSphere() // TODO: prenosenje transformComponent-a iz jednog collider-a u drugi pri set-ovanja novog collidera
{
    if(pCollider != nullptr)
        delete pCollider;
    
    pCollider = new ColliderSphere(pTransformComponent);
    return (ColliderSphere*)pCollider;
}

ColliderMesh* Rigidbody::setColliderMesh(const Mesh* pMesh)
{
    if(pCollider != nullptr)
        delete pCollider;
    
    pCollider = new ColliderMesh(pTransformComponent);
    ((ColliderMesh*)pCollider)->pMesh = pMesh;

    return (ColliderMesh*)pCollider;
}

void log(const Rigidbody& rigidbody)
{
    cout << "Rigidbody: ---" << endl;
    cout << "total energy: " << rigidbody.getTotalEnergy() << endl;
    cout << "mass: " << rigidbody.getMass() << endl;
    cout << "gravity: " << rigidbody.gravity << endl;
    cout << "restitution: " << rigidbody.restitution << endl;
    cout << "fixedTranslation: " << rigidbody.fixedTranslation << endl;
    cout << "fixedRotation: " << rigidbody.fixedRotation << endl;

    cout << "velocity: ";
    log(rigidbody.velocity);

    cout << "force: ";
    log(rigidbody.force);

    cout << "angularVelocity: " << endl;
    log(rigidbody.angularVelocity);

    cout << "torque: " << endl;
    log(rigidbody.torque);

    cout << "inertiaTensor: " << endl;
    log(rigidbody.getInertiaTensor());


    cout << "---" << endl;
}