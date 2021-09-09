#include"rv_pch.hpp"

#include"window.h"
#include"event/dispatcher.h"
#include"event/event.h"

static void GlfwErrorCallback(int error, const char* description)
{
    // Ignoring common, maybe unavoidable erros on wayland, glfw still works even when they occur
    RV_ASSERT(strncmp(description, "Wayland: The platform does not support setting the input focus", 10) || strncmp(description, "Wayland: focusing a window requires user interaction", 10),
    "Glfw Error: " << description);
}

void Window::onUpdate()
{
    if(Input::get()->isKeyPressed(RV_KEY_Q))
    {
        EventWindowClose event;
        EventDispatcher::dispatch(&event);
    }

    glfwSwapBuffers(pWindow);
    glfwPollEvents();
}

void Window::shutdown()
{
    glfwDestroyWindow(pWindow);
    glfwTerminate();
}

void Window::setVSync(bool isEnabled)
{
    if(isEnabled)
    {
        glfwSwapInterval(1);
    }
    else
    {
        glfwSwapInterval(0);
    }
    m_Data.vSync = isEnabled;
}

bool Window::getVSync()
{
    return m_Data.vSync;
}

unsigned int Window::getWidth() const
{
    return m_Data.width;
}

unsigned int Window::getHeight() const
{
    return m_Data.height;
}

static void sizeCallback(GLFWwindow* pWindow, int width, int height)
{
    EventWindowResize event(width, height);
    EventDispatcher::dispatch(&event);
}

static void closeCallback(GLFWwindow* pWindow)
{
    EventWindowClose event;
    EventDispatcher::dispatch(&event);
}

static void cursorPosCallback(GLFWwindow* pWindow, double xPosition, double yPosition)
{
    EventMouseMoved event(Vec2f(xPosition, yPosition));
    EventDispatcher::dispatch(&event);
}

void Window::init(bool enableVSync, bool isFullscreen, unsigned int windowWidth, unsigned int windowHeight, const std::string& windowTitle)
{

    m_Data.title = windowTitle;
    m_Data.width = windowWidth;
    m_Data.height = windowHeight;
    m_Data.isFullscreen = isFullscreen;

    static bool s_GlfwInitialized = false;
    if(!s_GlfwInitialized)
    {
        bool success = glfwInit();
        RV_ASSERT(success, "could not initialize GLFW");

        s_GlfwInitialized = true;
        glfwSetErrorCallback(GlfwErrorCallback);
    }
    else {
        RV_ASSERT(false, "reviv currently does not support creating multiple windows");
    }

    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

#ifdef RV_DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

    //glfwWindowHint(GLFW_SAMPLES, 4); // for MSAA anti-aliasing

    GLFWmonitor* monitor = nullptr;

    if(m_Data.isFullscreen){
        m_Data.height = mode->height;
        m_Data.width = mode->width;
        m_Data.refreshRate = mode->refreshRate;
        monitor = glfwGetPrimaryMonitor();
    }

    pWindow = glfwCreateWindow(m_Data.width, m_Data.height, m_Data.title.c_str(), monitor, nullptr);
    
    if (!pWindow)
    {
        std::cout << "ERROR: Failed to create GLFW window" << std::endl;
        shutdown();
        RV_ASSERT(false, "");
    }	

    glfwMakeContextCurrent(pWindow);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        RV_ASSERT(false, "Failed to initialize GLAD");
    }

    //cout << "Vendor: " << glGetString(GL_VENDOR) << endl;
    //cout << "Renderer: " << glGetString(GL_RENDERER) << endl;
    //cout << "Version: " << glGetString(GL_VERSION) << endl;

    //glfwSetFramebufferSizeCallback(pWindow, framebufferSizeCallback);

    //glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //glfwSetInputMode(pWindow, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);

    setVSync(enableVSync);

    ///m_Data.m_EventCallback = eventCallbackFunction;

    glfwSetWindowUserPointer(pWindow, &m_Data);

    glfwSetWindowSizeCallback(pWindow, sizeCallback);
    glfwSetWindowCloseCallback(pWindow, closeCallback);
    //glfwSetKeyCallback(pWindow, keyCallback);
    //glfwSetMouseButtonCallback(pWindow, mouseButtonCallback);
    //glfwSetScrollCallback(pWindow, scrollCallback);
    glfwSetCursorPosCallback(pWindow, cursorPosCallback);
    //glfwSetCursorEnterCallback(pWindow, cursorEnterCallback);
}

float Window::getRatio()
{
    return (((float)m_Data.width) / ((float)m_Data.height));
}
