#pragma once

class Application
{
public:
    Application() = default;
    virtual ~Application() = default;
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    void run();
};

Application* createApplication();