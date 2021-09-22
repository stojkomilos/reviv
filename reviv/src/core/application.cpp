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

void Application::onEvent(Event* event)
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

    RV_ASSERT(sizeof(Mat<4,4>) == sizeof(float) * 16, "invalid matrix size. there can be NO other data Mat<> instances except the matrix data itself");

    cout << "Initializing Reviv engine" << endl;

    window.init(true, !RV_DEBUG, 1600, 900, applicationName);              // window.init() must go before RenderManager::init()

    Input::get()->disableCursor();        // For the first person 3d camera controller
    Input::get()->useRawMouseMotion();

    PhysicsManager::get()->init();
    RenderManager::get()->init();

    AssetManager::get()->init();
    GameStuffManager::get()->init();
    Time::get()->init(); // must be last (so that physics doesn't start before everything is loaded)

    cout << "Completed initializing reviv engine" << endl;
}

void Application::run()
{
    m_IsRunning = true;

    initEngine();                       // engine defined init function

    cout << "Initializing application" << endl;
    initAfterEngine();                  // game defined init funciton
    cout << "Completed initializing application" << endl;

    while(m_IsRunning)
    {
        onUpdate();                     // game defined onUpdate function

        float dt = Time::get()->getDelta();

        Time::get()->onUpdate();
        Input::get()->onUpdate(dt);
        GameStuffManager::get()->onUpdate(dt);
        PhysicsManager::get()->onUpdate(dt);             //TODO: maybe goes before rendermanager? probably doesn't matter too much
        RenderManager::get()->onUpdate();
        window.onUpdate();              // update the glfw window
    }

    RenderManager::get()->shutdown();
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