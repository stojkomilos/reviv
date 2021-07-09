#include"physics_manager.h"
#include"renderer/camera.h" //TODO ukloni

#include"scene/scene.h"
#include"scene/components.h"

extern Entity* stanic;
extern Mat4 identity; //TODO, ukloniti

void PhysicsManager::updateTransforms() // updates the transforms of all the entities according to the positions and stuff
{
    for(const Entity& entity : *Scene::getEntityList())
    {
        if(entity.has<TransformComponent>() and entity.has<PositionComponent>())
        {
            cout << "Updating tranform for entity: " << entity.entityName << endl;
            *entity.get<TransformComponent>() = translate(identity, *entity.get<PositionComponent>());
        }
    }
}

void PhysicsManager::alignPositionAndRotation(const Entity& parent, Entity* child)
{

    bool hasParentPosition = parent.has<PositionComponent>();
    bool hasParentRotation = parent.has<RotationComponent>();
    bool hasChildPosition = child->has<PositionComponent>();
    bool hasChildRotation = child->has<RotationComponent>();

    auto parentPosition = parent.get<PositionComponent>();
    auto parentRotation = parent.get<RotationComponent>();
    auto childPosition = child->get<PositionComponent>();
    auto childRotation = child->get<RotationComponent>();

    if(hasParentPosition and hasParentRotation and hasChildPosition and hasChildRotation){
        childPosition = parentPosition;
        childRotation = parentRotation;
    }
    else {
        cout << "ERROR: Required components are not present in entity\n";
        assert(false);
    }
}

void PhysicsManager::update()
{
    PositionComponent* stanicPos = stanic->get<PositionComponent>();
    auto* player = Scene::getPlayerEntity(3);
    Entity* kurac = &((*Scene::getEntityList())[3]);

    cout << "PP ISPIS: " << endl;
    cout << "NrComponents: player: " <<  Scene::getPlayerEntity(4)->components.size() << " Stanic: " << stanic->components.size() << " Camera:" << Scene::getCameraEntity()->components.size() << " kurac: " << kurac->components.size() << endl;
    cout << "Names Stanic: " << stanic->entityName << " Camera:" << Scene::getCameraEntity()->entityName << " kurac: " << kurac->entityName << " ";
    cout << "player: " <<  Scene::getPlayerEntity(5)->entityName << endl;
    cout << "PP KRaj ispisa: " << endl;

    auto* playerPos = Scene::getPlayerEntity(6)->get<PositionComponent>();
    *stanicPos = add(*playerPos, Vec3f(5 * sin(glfwGetTime()), 0, 5 * cos(glfwGetTime())));

    // ---
    cout << "Stanic position: ";
    log(*stanic->get<PositionComponent>());
    cout << "Player position: ";
    log(*Scene::getPlayerEntity(7)->get<PositionComponent>());
    cout << "Player rotation: ";
    log(*Scene::getPlayerEntity(8)->get<RotationComponent>());

    cout << "Camera position: ";
    log(*Scene::getCameraEntity()->get<PositionComponent>());
    cout << "Camera viewmatrix";
    log(Scene::getCameraEntity()->get<CameraComponent>()->camera.viewMatrix);
    cout << "Camera projectionmatrix";
    log(Scene::getCameraEntity()->get<CameraComponent>()->camera.projectionMatrix);
    // ---

    alignPositionAndRotation(*Scene::getPlayerEntity(9), Scene::getCameraEntity());

    updateTransforms();
}