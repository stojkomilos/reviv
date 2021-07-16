#include"application.h"
#include"renderer/render_manager.h"
#include"physics/physics_manager.h"
#include"core/time.h"

Application* Application::s_Instance(nullptr);

Application::Application(const std::string& applicationName /* = "Reviv App" */ )
    : applicationName(applicationName)
{
    assert(s_Instance == nullptr); // ERROR: can't have more than 1 application

    s_Instance = this;
}

void Application::initEngine()
{
    window.init(false, false, 1600, 900, applicationName);              // window.init() must go before RenderManager::init()

    PhysicsManager::init();
    RenderManager::init(window.m_Data);
}

void Application::run()
{
    initBeforeEngine();                 // game defined init funciton
    initEngine();                       // engine defined init function
    initAfterEngine();                  // game defined init funciton

    while(true)
    {
        onUpdate();                     // game defined onUpdate function

        Time::onUpdate();
        PhysicsManager::onUpdate();
        RenderManager::onUpdate(window.m_Data);
        window.onUpdate();              // update the glfw window
    }
}

Window* Application::getWindow()
{
    return &window;
}
