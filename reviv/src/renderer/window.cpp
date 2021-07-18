#include<assert.h>

#include"window.h"
#include"events/dispatcher.h"

#include<cstring>

//#include"core/input.h" //TODO: ukloni ovo kada ubacis event system

static void framebuffer_size_callback(GLFWwindow* window, int width, int height) //TODO
{ 
    glViewport(0, 0, width, height);
}

static void GlfwErrorCallback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error: %s\n", description);

    // Common, maybe unavoidable erros on wayland, glfw still works even when they occur
    assert(strncmp(description, "Wayland: The platform does not support setting the input focus", 10) or strncmp(description, "Wayland: focusing a window requires user interaction", 10));
}

void Window::onUpdate()
{
    if(Input::isKeyPressed(RV_KEY_Q))
        Application::getInstance()->m_IsRunning = false;

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
    //WindowData* data = (WindowData*)glfwGetWindowUserPointer(pWindow);
    EventMouseMoved event(Vec2f(xPosition, yPosition));
    EventDispatcher::dispatch(&event);
    //cout << "Cursor pos callback, x: " << xPosition << " y: " << yPosition << endl;

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
        assert(success); // ERROR: could not initialize GLFW

        s_GlfwInitialized = true;
        glfwSetErrorCallback(GlfwErrorCallback);
    }
    else {
        assert(false); // ERROR: reviv currently does not support creating multiple windows
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // TODO: vidi sta ovo radi, i da li moze da se upali na wayland


	if(m_Data.isFullscreen){
		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		m_Data.height = mode->height;
		m_Data.width = mode->width;
	}
    
	pWindow = glfwCreateWindow(m_Data.width, m_Data.height, m_Data.title.c_str(), NULL, NULL);
    
    if (!pWindow) {
        std::cout << "ERROR: Failed to create GLFW window" << std::endl;
        shutdown();
        assert(false);
    }	

    if (!m_Data.isFullscreen) //TODO: idk sta je ovo
	{
		glfwSetWindowPos(pWindow, -m_Data.width - 5, 0); //TODO: NE RADI NA WAYLANDS MOZDA NAVODNO
	}

	glfwMakeContextCurrent(pWindow);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		assert(false);
	}

	cout << "Vendor: " << glGetString(GL_VENDOR) << endl;
	cout << "Renderer: " << glGetString(GL_RENDERER) << endl;
	cout << "Version: " << glGetString(GL_VERSION) << endl;

	glViewport(0, 0, m_Data.width, m_Data.height);
	glfwSetFramebufferSizeCallback(pWindow, framebuffer_size_callback);

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

static void openGlLogMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParams)
{
    cout << endl;
    switch(severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:
            cout << "[OpenGL Debug HIGH] " << message << endl;
            assert(false);
            break;

        case GL_DEBUG_SEVERITY_MEDIUM:
            cout << "[OpenGL Debug MEDIUM] " << message << endl;
            assert(false);
            break;

        case GL_DEBUG_SEVERITY_LOW:
            cout << "[OpenGL Debug LOW] " << message << endl;
            assert(false);
            break;

        case GL_DEBUG_SEVERITY_NOTIFICATION:
            cout << "[OpenGL Debug NOTIFICATION] " << message << endl;
            break;

        default:
            cout << "ERROR: opengl returned unknown debug error SEVERITY" << endl;
            assert(false);
    }
}