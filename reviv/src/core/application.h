#pragma once

#include"renderer/window.h"

class Application
{
public:
    virtual ~Application() = default;
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    void initEngine();
    virtual void init() = 0;
    virtual void onUpdate() = 0;

    void run();
    Window window;

protected:
    Application() = default;
};

Application* createApplication();