#include"physics_manager.h"
#include<GLFW/glfw3.h>
#include"renderer/camera.h" //TODO ukloni

extern std::vector<Entity> gEntityList;
extern Mat4 identity; //TODO, ukloniti
extern Entity* gpPlayerEntity;
extern Entity* gpCameraEntity;

void PhysicsManager::updateTransforms() // updates the transforms of all the entities according to the positions and stuff
{
    for(const Entity& entity : gEntityList)
    {
        if(entity.hasComponent<Transform>() and entity.hasComponent<PositionComponent>())
        {
            cout << "Updating tranform for entity: " << entity.name << endl;
            *entity.getComponent<Transform>() = translate(identity, *entity.getComponent<PositionComponent>());
        }
    }
}

void PhysicsManager::alignPositionAndRotation(const Entity& parent, Entity* child)
{

    bool parentPosition = parent.hasComponent<PositionComponent>();
    bool parentRotation = parent.hasComponent<RotationComponent>();

    bool childPosition = child->hasComponent<PositionComponent>();
    bool childRotation = child->hasComponent<RotationComponent>();

    if(parentPosition and parentRotation and childPosition and childRotation){
        *(*child).getComponent<PositionComponent>() = *parent.getComponent<PositionComponent>();
        child->getComponent<RotationComponent>()->pitch = parent.getComponent<RotationComponent>()->pitch;
        child->getComponent<RotationComponent>()->yaw = parent.getComponent<RotationComponent>()->yaw;
        child->getComponent<RotationComponent>()->roll = parent.getComponent<RotationComponent>()->roll;
    }
    else {
        cout << "ERROR: Required components are not present in entity\n";
        assert(false);
    }

    //std::cout << "pitch = " << pitch << " " << yaw << " " << roll << "\n";
}

void PhysicsManager::update()
{
    *gEntityList[2].getComponent<PositionComponent>() = add(*gpPlayerEntity->getComponent<PositionComponent>(), Vec3f(5 * sin(glfwGetTime() * 5), 0, 5 * cos(glfwGetTime() * 5)));

    // ---
    cout << "Stanic position: ";
    log(*gEntityList[2].getComponent<PositionComponent>());
    cout << "Player position: ";
    log(*gpPlayerEntity->getComponent<PositionComponent>());
    cout << "Player rotation: ";
    log(*gpPlayerEntity->getComponent<RotationComponent>());

    cout << "Camera position: ";
    log(*gpCameraEntity->getComponent<PositionComponent>());
    cout << "Camera viewmatrix";
    log(gpCameraEntity->getComponent<PerspectiveCamera>()->viewMatrix);
    cout << "Camera projectionmatrix";
    log(gpCameraEntity->getComponent<PerspectiveCamera>()->projectionMatrix);
    // ---

    alignPositionAndRotation(*gpPlayerEntity, gpCameraEntity);

    updateTransforms();
}