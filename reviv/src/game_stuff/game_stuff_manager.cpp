#include"game_stuff_manager.h"

void GameStuffManager::iInit()
{
    Entity* camera = Scene::setCameraEntity(Scene::createEntity("Camera"));
    camera->add<CameraComponent>(0.01f, 5000.f, degreesToRadians(100.f));

    Entity* player = Scene::setPlayerEntity(Scene::createEntity("Player"));
}

void GameStuffManager::iOnUpdate()
{
    weather.onUpdate();
}