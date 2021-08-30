#include"dynamics_manager.h"

#include"scene/scene.h"
#include"scene/components.h"

void DynamicsManager::onUpdate(float dt)
{
    for(auto itEntity = Scene::getEntityList()->begin(); itEntity != Scene::getEntityList()->end(); itEntity++)
    {
        if(!Scene::isEntityValid(&(*itEntity)))
            continue;
        if(!itEntity->has<PhysicalComponent>())
            continue;

        //cout << "Updating dynamics for entity: " << itEntity->entityName << endl;

        auto* pPhysical = &itEntity->get<PhysicalComponent>()->physical;
        auto* pTransform = itEntity->get<TransformComponent>();

        pPhysical->force += pPhysical->mass * pPhysical->gravity * Vec3f(0, 0, -1);

        pPhysical->velocity += pPhysical->force / pPhysical->mass * dt;
        pTransform->position += pPhysical->velocity * dt;

        pPhysical->force = Vec3f(0, 0, 0);
    }
}

PhysicalDynamic::PhysicalDynamic()
    : mass(1.f), velocity{0, 0, 0}, force{0, 0, 0}
{
    gravity = Scene::getGravity();
}

void log(const PhysicalDynamic physical)
{
    cout << "mass: " << physical.mass << endl;

    cout << "velocity: ";
    log(physical.velocity);

    cout << "force: ";
    log(physical.force);
}