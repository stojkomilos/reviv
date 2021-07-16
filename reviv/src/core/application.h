#pragma once

#include<assert.h>
#include"renderer/window.h"

class Application
{
public:
    virtual ~Application() = default;
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    static Application* getInstance()
    {
        return s_Instance;
    }
    
    Window* getWindow();

protected:
    Application(const std::string& applicationName = "Reviv App");

    virtual void initBeforeEngine() = 0;
    virtual void initAfterEngine() = 0;
    virtual void onUpdate() = 0;

    void run();

    void initEngine();

private:
    std::string applicationName;
    friend int main(int argc, char** argv);
    static Application* s_Instance;
    Window window;
};

Application* createApplication();