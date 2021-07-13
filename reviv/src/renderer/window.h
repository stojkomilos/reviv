#pragma once

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<string>
#include<iostream>

using std::cin; using std::cout; using std::endl;

class Window
{
public:
    Window() = default;
    ~Window();
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    void init(bool enableVSync = false, bool isFullScreen = false, unsigned int windowWidth = 1600, unsigned int windowHeight = 900, const std::string& windowTitle = "Reviv");
    void shutdown();
    void onUpdate();
    unsigned int getWidth();
    unsigned int getHeight();
    void setVSync(bool isEnabled);
    bool getVSync();
    GLFWwindow* pWindow;

private:
    unsigned int m_Width;
    unsigned int m_Height;
    std::string m_Title;
    bool m_VSync;
    bool m_IsFullscreen;
};