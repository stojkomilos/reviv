#include"game_stuff_manager.h"
#include"scene/components.h"

void GameStuffManager::init()
{
    Entity* camera = Scene::setCameraEntity(Scene::createEntity("Camera"));
    camera->add<CameraComponent>(0.01f, 5000.f, degreesToRadians(100.f));

    Entity* player = Scene::setPlayerEntity(Scene::createEntity("Player"));
}

void GameStuffManager::onUpdate()
{
    weather.onUpdate();
}