#include<assert.h>

#include"window.h"

static void framebuffer_size_callback(GLFWwindow* window, int width, int height) //TODO
{ 
	glViewport(0, 0, width, height);
}

static void GlfwErrorCallback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error: %s\n", description);
    assert(description != "Wayland: The Platform does not support setting the input focus" and description != "Wayland: focusing a window requires user interaction"); // Common, maybe unavoidable erros on wayland, glfw still works even when they occur
}

void Window::onUpdate()
{
    if(glfwWindowShouldClose(pWindow))
    {
        shutdown();
    }

    glfwSwapBuffers(pWindow);
    glfwPollEvents();
}

void Window::shutdown()
{
    glfwDestroyWindow(pWindow);
    glfwTerminate();
}

Window::~Window()
{
    shutdown();
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
    //assert(false);
    WindowData* data = (WindowData*)glfwGetWindowUserPointer(pWindow);

    data->width = width;
    data->height = height;

    //WindowResizeEvent event(width, height);
    //data->eventCallback(event);

    cout << "RESIZE width: " << width << " height: " << height << endl;
}

static void closeCallback(GLFWwindow* pWindow)
{
    //assert(false);

    WindowData *data = (WindowData*)glfwGetWindowUserPointer(pWindow);

    //WindowCloseEvent event;
    //data->eventCallback(event);

    cout << "CLOSE callback" << endl;
}

static void keyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mods)
{
    //assert(false);
    WindowData* data = (WindowData*)glfwGetWindowUserPointer(pWindow);

    cout << "KEY callback" << endl;

    switch(action)
    {
        case GLFW_PRESS:
        {
            cout << "Press" << endl;
            //KeyPressedEvent event(key, 0);
            //data->eventCallback(event);
            break;
        }
        case GLFW_RELEASE:
        {
            cout << "Release" << endl;
            //KeyReleasedEvent event(key);
            //data->eventCallback(event);
            break;
        }
        case GLFW_REPEAT:
        {
            cout << "Repeat" << endl;
            //KeyReleasedEvent event(key, 1);
            //data->eventCallback(event);
            break;
        }
        default:
        {
            assert(false); // ERROR: glfw event not recognized
        }
    }
}

static void mouseButtonCallback(GLFWwindow* pWindow, int button, int action, int modes)
{
    WindowData* data = (WindowData*)glfwGetWindowUserPointer(pWindow);
    cout << "Mouse button" << endl;

}

static void scrollCallback(GLFWwindow* pWindow, double xOffset, double yOffset)
{
    WindowData* data = (WindowData*)glfwGetWindowUserPointer(pWindow);
    cout << "Scroll" << endl;
}

static void cursorPosCallback(GLFWwindow* pWindow, double xPosition, double yPosition)
{
    WindowData* data = (WindowData*)glfwGetWindowUserPointer(pWindow);
    cout << "Cursor pos callback, x: " << xPosition << " y: " << yPosition << endl;

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

    glfwSetWindowUserPointer(pWindow, &m_Data);

    glfwSetWindowSizeCallback(pWindow, sizeCallback);
    glfwSetWindowCloseCallback(pWindow, closeCallback);
    glfwSetKeyCallback(pWindow, keyCallback);
    glfwSetMouseButtonCallback(pWindow, mouseButtonCallback);
    glfwSetScrollCallback(pWindow, scrollCallback);
    glfwSetCursorPosCallback(pWindow, cursorPosCallback);
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