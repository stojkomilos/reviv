#include"application.h"
#include"renderer/render_manager.h"
#include"physics/physics_manager.h"

void Application::initEngine()
{
    PhysicsManager::init();
    window.init(); // window.init() must go before RenderManager::init()
    RenderManager::init();
}

void Application::run()
{
    init();                 // game defined init funciton
    initEngine();           // engine defined init function

    while(true)
    {
        onUpdate();         // game defined onUpdate function

        PhysicsManager::onUpdate();
        RenderManager::onUpdate();
        window.onUpdate();          // update the glfw window
    }
}