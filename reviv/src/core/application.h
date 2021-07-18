#pragma once

#include"rv_pch.hpp"

#include"renderer/window.h"
#include"core/input.h"
#include"events/event.h"

class Application
{
public:
    virtual ~Application() = default;
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    static void onEvent(Event* event) { getInstance()->iOnEvent(event); }
    static Application* getInstance()
    {
        return s_Instance;
    }
    
    Window* getWindow();
    bool m_IsRunning;

protected:
    Application(const std::string& applicationName = "Reviv App");

    virtual void initBeforeEngine() = 0;
    virtual void initAfterEngine() = 0;
    virtual void onUpdate() = 0;


    void run();

    void initEngine();

private:
    friend int main(int argc, char** argv);
    static Application* s_Instance;
    std::string applicationName;
    Window window;
    void iOnEvent(Event* event);

    void onEventWindowClose(Event* event);
    void onEventWindowResize(Event* event);
};

Application* createApplication();