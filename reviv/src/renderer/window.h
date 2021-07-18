#pragma once

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<string>
#include<iostream>

#include"events/event.h"

using std::cin; using std::cout; using std::endl;

struct WindowData
{
    std::string title;
    unsigned int width;
    unsigned int height;
    bool vSync;
    bool isFullscreen;

    void(*eventCallback)(Event*);
};

class Window
{
public:
//    typedef void(Application::*eventCallbackFunction)(Event*);

    Window() = default;
    ~Window() = default;
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    void init(bool enableVSync, bool isFullScreen, unsigned int windowWidth, unsigned int windowHeight, const std::string& windowTitle);
    void onUpdate();
    void shutdown();
    unsigned int getWidth() const;
    unsigned int getHeight() const;
    void setVSync(bool isEnabled);
    bool getVSync();

    GLFWwindow* pWindow;
    WindowData m_Data;
private:
};