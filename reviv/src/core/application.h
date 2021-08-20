#pragma once

#include"rv_pch.hpp"

#include"renderer/window.h"
#include"core/input.h"
#include"event/event.h"
#include"scene/asset_manager.h"
#include"game_stuff/game_stuff_manager.h"

class Application
{
public:
    virtual ~Application() = default;
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    static void onEvent(Event* event) { get()->iOnEvent(event); }
    static Application* get()
    {
        return s_Instance;
    }
    
    Window* getWindow();

    inline unsigned int getWindowWidth() { return window.m_Data.width; }
    inline unsigned int getWindowHeight() { return window.m_Data.height; }
    inline float getWindowRatio() { return window.getRatio(); }

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