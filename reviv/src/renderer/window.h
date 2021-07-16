#pragma once

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<string>
#include<iostream>

using std::cin; using std::cout; using std::endl;

struct WindowData
{
    std::string title;
    unsigned int width;
    unsigned int height;
    bool vSync;
    bool isFullscreen;
};

class Window
{
public:
    Window() = default;
    ~Window();
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    void init(bool enableVSync, bool isFullScreen, unsigned int windowWidth, unsigned int windowHeight, const std::string& windowTitle);
    void shutdown();
    void onUpdate();
    unsigned int getWidth() const;
    unsigned int getHeight() const;
    void setVSync(bool isEnabled);
    bool getVSync();

    GLFWwindow* pWindow;
    WindowData m_Data;
private:
};