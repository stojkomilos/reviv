#include"render_manager.h"

#ifdef NDEBUG
#define __REVIV_RELEASE__ 1
#else
#define __REVIV_RELEASE__ 0
#endif

/// -----
static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
//    assert(false);
}
/// ----

extern int gGameLoopCounter;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

extern Entity* stanic;
extern Entity* player;


RenderCommand gRenderCommand;
int RenderManager::startUp(int windowWidth1, int windowHeight1)
{

	windowWidth = windowWidth1;
	windowHeight = windowHeight1;

    glfwSetErrorCallback(error_callback);

	if(!glfwInit())
    {
        cout << "ERROR: could not initialize GLFW" << endl;
        assert(false);
    }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (__REVIV_RELEASE__)
	{
		fullscreen = true;
	}
	if(fullscreen){
		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		windowHeight = mode->height;
		windowWidth = mode->width;
	}

    cout << "KANCER PRE IME: " << player->entityName << endl;
	window = glfwCreateWindow(windowWidth, windowHeight, "OpenGL", NULL, NULL);

    if (!window) {
        std::cout << "ERROR: Failed to create GLFW window" << std::endl;
        glfwTerminate();
        assert(false);
        return -1;
    }	

    cout << "EVO GAA KANCER:" << endl;
    cout << "KANCER POSLE IME: " << player->entityName << endl;

    cout << "KRAJ IMENA\n";
    if (!fullscreen)
	{
	//	glfwSetWindowPos(window, -windowWidth - 5, 0); NE RADI NA WAYLANDS MOZDA NAVODNO
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		assert(false);
		return -1;
	}
	
	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;

	glViewport(0, 0, windowWidth, windowHeight);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE); ///pale se sticky mouse 
	//glfwSetCursorPosCallback(window, mouse_callback); TODOOO

    /// -------- MATERIALS ------------------------------------------------------------

	praviMono.setUp("../resources/shaders/monocolor.vs", "../resources/shaders/monocolor.fs");
    Shader* textureShader = new Shader; // TODO, memory leak
	textureShader->setUp("../resources/shaders/texture.vs", "../resources/shaders/texture.fs");
    auto* stanicMat = stanic->add<MaterialComponent>();
    *stanicMat = Material(*textureShader);

    /// -----
	
	std::vector<BufferElement> tempVboLayout1 = {
		{ShaderDataType::Float3, "a_Position", false},
		{ShaderDataType::Float2, "a_TexCoord", false},
		{ShaderDataType::Float3, "a_Normal",   false},
	};
	BufferLayout vboLayout1(tempVboLayout1);

	assert(sizeof(float) * 3 == sizeof(Vec3f));
	assert(sizeof(int) * 3 == sizeof(TripletOfInts));

	int nrAttributes;											 //samo jedna provere dal ima otprilike dovoljno atrib pointera
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;
	assert(nrAttributes >= 10); 

    ///
    auto* stanicVao = stanic->add<VaoComponent>();
    auto* stanicModel = stanic->get<ModelLoaderComponent>();
    stanicVao->vao.setUp();
    stanicVao->vao.bind();

    stanicVao->vao.vbo.layout = vboLayout1;
    stanicVao->vao.vbo.setUp((void*)stanicModel->modelLoader.pointer, stanicModel->modelLoader.nrTriangles * 3 * (2 * sizeof(Vec3f) + sizeof(Vec2f)), 0);
    stanicVao->vao.vbo.bind();
    stanicVao->vao.addVertexBuffer(stanic->get<VaoComponent>()->vao.vbo);
    ///

    glDebugMessageCallback(openGlLogMessage, nullptr);
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); // enables you too look in the call stack
   
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	stanicTexture.setUp("../resources/textures/stene.png");
    stanicTexture.bind(0);
	//beloTexture.setUp("../resources/textures/belo.png");

	Scene::getCameraEntity()->get<CameraComponent>()->camera.setUp(0.1f, renderDistance, 60.0f / 180.0f * 2.0f * 3.14f, ((float)(windowWidth)) / ((float)(windowHeight)));
	Scene::getCameraEntity()->get<CameraComponent>()->camera.recalculateProjectionMatrix();

	return 0;
}

int RenderManager::render()
{
	gRenderCommand.setClearColor(Vec4f(0.0f, 0.0f, 0.0f, 0.9f));
	gRenderCommand.clear();

	beginScene();

	for(const Entity& entity : *Scene::getEntityList())
    {
		if(entity.valid)
		{
            //cout << "ID Material: " << Material::id << endl;
            //cout << "ID Transform: " << Transform::id << endl;
            //cout << "ID Vao: " << Vao::id << endl;

			if(entity.has<MaterialComponent>() and entity.has<TransformComponent>() and entity.has<VaoComponent>())
			{
                cout << "Rendering entity: " << entity.entityName << endl;
				submit(
					(Material*)entity.get<MaterialComponent>(), 
					*entity.get<TransformComponent>(), 
					*entity.get<VaoComponent>());
			}
		}
	}


	glfwSwapBuffers(window); // abstract mozda informacija ima u cherno video-ima: game engine window
	glfwPollEvents();

	endScene();
	
	return 0;
}

void RenderManager::submit(Material* material, const Mat4& transform, const Vao& vao)
{
    // PRE SETA MORA DA SHADER BUDE BOUNDOVAN
    material->shader.bind();

    material->set("u_Color", Vec4f(1, 0, 0, 1));

    material->set("u_Projection", Scene::getCameraEntity()->get<CameraComponent>()->camera.projectionMatrix);

    material->set("u_View", Scene::getCameraEntity()->get<CameraComponent>()->camera.viewMatrix);

    material->set("u_Model", transform);

    material->set("u_Texture", 0);

    material->bind();
    // TODOOO -> material uniforme environment specific
    vao.bind();
    gRenderCommand.drawArrays(vao);
}

int RenderManager::shutDown() 
{
	
	delete[] voxelBuffer;

	glfwTerminate();
	return 0;
}

RenderManager::RenderManager() 
{
	nrOfValidCubes = sizeOfVoxelBuffer = 0;
	voxelBuffer = nullptr;
	renderHitbox = 0;
	windowWidth = windowHeight = 0;
	window = nullptr;
}

void RenderManager::beginScene()
{
    Camera* camera = &Scene::getCameraEntity()->get<CameraComponent>()->camera;

    if(!(Scene::getCameraEntity()->has<CameraComponent>() and Scene::getCameraEntity()->has<PositionComponent>() and Scene::getCameraEntity()->has<RotationComponent>()))
    {
        cout << "ERROR: submitted entity is supposed to be a camera, but does NOT have required components" << endl;
        assert(false);
    }
    else {
        cout << "Camera entity: " << Scene::getCameraEntity()->entityName << endl;
    }

	camera->recalculateViewMatrix(
        *Scene::getCameraEntity()->get<PositionComponent>(),
        *Scene::getCameraEntity()->get<RotationComponent>());

}
void RenderManager::endScene()
{

}
void RenderManager::openGlLogMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParams)
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