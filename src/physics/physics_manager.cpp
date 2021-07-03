#include"physics_manager.h"

extern std::vector<Entity*> gEntityList;
extern Mat4 identity; //TODO, ukloniti
extern Entity* gPlayerEntity, gCameraEntity;

void PhysicsManager::updateTransforms() // updates the transforms of all the entities according to the positions and stuff
{
    for(Entity* entity : gEntityList)
    {
        if(entity->hasComponent<TransformComponent>() and entity->hasComponent<PositionComponent>())
        {
            *entity->getComponent<TransformComponent>() = translate(identity, *entity->getComponent<PositionComponent>());
        }
    }
}


//    recalculateViewMatrix();
void PhysicsManager::alignPositionAndRotation(const Entity& parent, Entity& child)
{

    bool parentPosition = parent.hasComponent<PositionComponent>();
    bool parentRotation = parent.hasComponent<RotationComponent>();

    bool childPosition = child.hasComponent<PositionComponent>();
    bool childRotation = child.hasComponent<RotationComponent>();

    if(parentPosition and parentRotation and childPosition and childRotation){
        *child.getComponent<PositionComponent>() = *parent.getComponent<PositionComponent>();
        child.getComponent<RotationComponent>()->pitch = parent.getComponent<RotationComponent>()->pitch;
        child.getComponent<RotationComponent>()->yaw = parent.getComponent<RotationComponent>()->yaw;
        child.getComponent<RotationComponent>()->roll = parent.getComponent<RotationComponent>()->roll;
    }
    else {
        cout << "ERROR: Required components are not present in entity\n";
        assert(false);
    }

    //std::cout << "pitch = " << pitch << " " << yaw << " " << roll << "\n";
}

void PhysicsManager::update()
{
    updateTransforms();

    alignPositionAndRotation(*gPlayerEntity, *gCameraEntity);


}