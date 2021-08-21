#include"application.h"
#include"renderer/render_manager.h"
#include"physics/physics_manager.h"
#include"core/time.h"

Application* Application::s_Instance(nullptr);

Application::Application(const std::string& applicationName /* = "Reviv App" */ )
    : applicationName(applicationName)
{
    RV_ASSERT(s_Instance == nullptr, "Can't have more than 1 application");

    s_Instance = this;
}

void Application::iOnEvent(Event* event)
{
    switch(event->m_Type)
    {
        case(EventType::EventTypeWindowClose):
            onEventWindowClose(event);
            break;
        case(EventType::EventTypeWindowResize):
            onEventWindowResize(event);
            break;
        default:
            RV_ASSERT(false, "event type not recognized by Application class");
    }
}

void Application::initEngine()
{
    RV_ASSERT(sizeof(char) == 1, "");
    RV_ASSERT(sizeof(unsigned char) == 1, "");

    RV_ASSERT(sizeof(int) == 4, "");
    RV_ASSERT(sizeof(unsigned int) == 4, "");

    RV_ASSERT(sizeof(long long) == 8, "");
    RV_ASSERT(sizeof(unsigned long long) == 8, "");

    RV_ASSERT(sizeof(float) == 4, "");
    RV_ASSERT(sizeof(double) == 8, "");
    RV_ASSERT(sizeof(bool) == 1, "");

    cout << "Initializing Reviv engine" << endl;

    window.init(true, !RV_DEBUG, 1600, 900, applicationName);              // window.init() must go before RenderManager::init()

    Input::disableCursor();        // For the first person 3d camera controller
    Input::useRawMouseMotion();

    PhysicsManager::init();
    RenderManager::init();

    AssetManager::get()->init();
    GameStuffManager::init();
}

void Application::run()
{
    m_IsRunning = true;

    initBeforeEngine();                 // game defined init funciton
    initEngine();                       // engine defined init function
    initAfterEngine();                  // game defined init funciton

    while(m_IsRunning)
    {
        onUpdate();                     // game defined onUpdate function

        Time::onUpdate();
        Input::onUpdate();
        PhysicsManager::onUpdate();
        //GameStuffManager::onUpdate();
        RenderManager::onUpdate();
        window.onUpdate();              // update the glfw window
    }

    RenderManager::shutdown();
    window.shutdown();
}

Window* Application::getWindow()
{
    return &window;
}

void Application::onEventWindowClose(Event* event)
{
    RV_ASSERT(event->m_Type == EventType::EventTypeWindowClose && event->getName() == "EventWindowClose", "Incorrect event type");
    m_IsRunning = false;
}

void Application::onEventWindowResize(Event* event)
{
    RV_ASSERT(event->m_Type == EventType::EventTypeWindowResize && event->getName() == "EventWindowResize", "Incorrect event type");
    EventWindowResize* ev = (EventWindowResize*)event;
    window.m_Data.width = ev->m_Width;
    window.m_Data.height = ev->m_Height;
}