#include<assert.h>

#include"window.h"

static void framebuffer_size_callback(GLFWwindow* window, int width, int height) //TODO
{ 
	glViewport(0, 0, width, height);
}

static void error_callback(int error, const char* description) ///TODO: samo paste
{
    fprintf(stderr, "Error: %s\n", description);
    assert(false);
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
    m_VSync = isEnabled;
}

bool Window::getVSync()
{
    return m_VSync;
}

void Window::init(bool enableVSync, bool isFullscreen, unsigned int windowWidth, unsigned int windowHeight, const std::string& windowTitle)
{
    m_Width = windowWidth;
    m_Height = windowHeight;
    m_Title = windowTitle;
    m_IsFullscreen = isFullscreen;

    glfwSetErrorCallback(error_callback);

    static bool s_GlfwInitialized = false;
    if(!s_GlfwInitialized)
    {
        bool success = glfwInit();
        assert(success); // ERROR: could not initialize GLFW

        s_GlfwInitialized = true;
    }
    else {
        assert(false); // ERROR: reviv currently does not support creating multiple windows
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // TODO: vidi sta ovo radi, i da li moze da se upali na wayland


	if(m_IsFullscreen){
		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		m_Height = mode->height;
		m_Width = mode->width;
	}
    
	pWindow = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), NULL, NULL);
    
    if (!pWindow) {
        std::cout << "ERROR: Failed to create GLFW window" << std::endl;
        shutdown();
        assert(false);
    }	

    if (!m_IsFullscreen) //TODO: idk sta je ovo
	{
		glfwSetWindowPos(pWindow, -m_Width - 5, 0); //TODO: NE RADI NA WAYLANDS MOZDA NAVODNO
	}

	glfwMakeContextCurrent(pWindow);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		assert(false);
	}

	cout << "Vendor: " << glGetString(GL_VENDOR) << endl;
	cout << "Renderer: " << glGetString(GL_RENDERER) << endl;
	cout << "Version: " << glGetString(GL_VERSION) << endl;

	glViewport(0, 0, m_Width, m_Height);
	glfwSetFramebufferSizeCallback(pWindow, framebuffer_size_callback);

	glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetInputMode(pWindow, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
	//glfwSetCursorPosCallback(window, mouse_callback); TODOOO

    setVSync(enableVSync);
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