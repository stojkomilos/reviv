#include"render_manager.h"

#ifdef NDEBUG
#define __REVIV_RELEASE__ 1
#else
#define __REVIV_RELEASE__ 0
#endif

Vao stanicVao;
Vbo stanicVbo;

//extern SimulationManager gSimulationManager;
//extern PhysicsManager gPhysicsManager;

extern std::vector<Entity> gEntityList;
extern Entity* gpPlayerEntity;
extern Entity* gpCameraEntity;

extern Mat4 identity;
extern int gGameLoopCounter;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

RenderCommand gRenderCommand;
int RenderManager::startUp(int windowWidth1, int windowHeight1)
{

	windowWidth = windowWidth1;
	windowHeight = windowHeight1;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (__REVIV_RELEASE__)
	{
		fullscreen = true;
	}
	if(fullscreen){
		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		windowHeight = mode->height;
		windowWidth = mode->width;
	}
	window = glfwCreateWindow(windowWidth, windowHeight, "OpenGL", NULL, NULL);
	if (!fullscreen)
	{
		glfwSetWindowPos(window, -windowWidth - 5, 0);
	}
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
		assert(false);
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

	praviMono.setUp("../resources/shaders/jedan.vs", "../resources/shaders/jedan.fs");
    //Material stanicMaterial(praviMono);
    gEntityList[2].addComponent<Material>(new Material(praviMono));


	//shaderTexture.setUp("../resources/shaders/texture.vs", "../resources/shaders/texture.fs");
	//shaderMonoChroma.setUp("../resources/shaders/mono_chroma.vs", "../resources/shaders/mono_chroma.fs");  

    /// ----------------------------------------------------------------------------
	
	std::vector<BufferElement> tempVboLayout1 = {
		{ShaderDataType::Float3, "a_Position", false},
		{ShaderDataType::Float2, "a_TexCoord", false},
		{ShaderDataType::Float3, "a_Normal",   false},
	};
	BufferLayout vboLayout1(tempVboLayout1);

	//sphereVao.setUp();
	//sphereVao.bind();
	
	//sphereVbo.layout = vboLayout1;
	//sphereVbo.setUp(sphere.pointer, sphere.nrTriangles * 3 * (2 * sizeof(Vec3f) + sizeof(Vec2)), 0);	
	//sphereVao.addVertexBuffer(sphereVbo);

	assert(sizeof(float) * 3 == sizeof(Vec3f));
	assert(sizeof(int) * 3 == sizeof(TripletOfInts));

	int nrAttributes;											 //samo jedna provere dal ima otprilike dovoljno atrib pointera
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;
	assert(nrAttributes >= 10); 

	//auto jedan = (*gStanic.getComponent<ModelLoader>()); TODOO


	//////
    // Konvertuj ovo u pravi Entity shit TODOO
	//gStanic.addComponent<Vao>(&stanicVao); // clan vao -> vbo
	//gStanic.addComponent<Vbo>(&stanicVbo);

	//gStanic.getComponent<Vao>()->setUp();
	//gStanic.getComponent<Vao>()->bind();

	//gStanic.getComponent<Vbo>()->layout = vboLayout1;
	//gStanic.getComponent<Vbo>()->setUp((void*)gStanic.getComponent<ModelLoader>()->pointer, gStanic.getComponent<ModelLoader>()->nrTriangles * 3 * (2 * sizeof(Vec3f) + sizeof(Vec2f)), 0);
	//gStanic.getComponent<Vbo>()->bind();
	//gStanic.getComponent<Vao>()->addVertexBuffer(*gStanic.getComponent<Vbo>());

    //Vao stanicVao;
    gEntityList[2].addComponent<Vao>(new Vao);
    gEntityList[2].getComponent<Vao>()->setUp();
    gEntityList[2].getComponent<Vao>()->bind();

    gEntityList[2].getComponent<Vao>()->vbo.layout = vboLayout1;
    gEntityList[2].getComponent<Vao>()->vbo.setUp((void*)gEntityList[2].getComponent<ModelLoader>()->pointer, gEntityList[2].getComponent<ModelLoader>()->nrTriangles * 3 * (2 * sizeof(Vec3f) + sizeof(Vec2f)), 0);
    gEntityList[2].getComponent<Vao>()->vbo.bind();
    gEntityList[2].getComponent<Vao>()->addVertexBuffer(gEntityList[2].getComponent<Vao>()->vbo);

	//cubeVao.setUp();
	//cubeVao.bind();
	
	//cubeVbo.layout = vboLayout1;
	//cubeVbo.setUp((void*)cube.pointer, cube.nrTriangles * 3 * (2 * sizeof(Vec3f) + sizeof(Vec2f)), 0);
	//cubeVbo.bind();
	//cubeVao.addVertexBuffer(cubeVbo);

	//////

    glDebugMessageCallback(openGlLogMessage, nullptr);
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); // enables you too look in the call stack
    /////


	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	stanicTexture.setUp("../resources/textures/stene.png");
	beloTexture.setUp("../resources/textures/belo.png");

	//---
	sceneData = new SceneData;
	
	gpCameraEntity->getComponent<Cameraa>()->setUp(0.1f, renderDistance, 60.0f / 180.0f * 2.0f * 3.14f, ((float)(windowWidth)) / ((float)(windowHeight)));
	gpCameraEntity->getComponent<Cameraa>()->recalculateProjectionMatrix();
	//camera.setUp(0.1f, renderDistance, 60.0f / 180.0f * 2.0f * 3.14f, ((float)(windowWidth)) / ((float)(windowHeight)));
	//camera.recalculateProjectionMatrix();

	///--

	return 0;
}

int RenderManager::render()
{
	gRenderCommand.setClearColor(Vec4f(0.0f, 0.0f, 0.0f, 0.9f));
	gRenderCommand.clear();

	beginScene(gpCameraEntity);

	for(const Entity& entity : gEntityList)
    {
		if(entity.valid)
		{
            //cout << "ID Material: " << Material::id << endl;
            //cout << "ID Transform: " << Transform::id << endl;
            //cout << "ID Vao: " << Vao::id << endl;

			if(entity.hasComponent<Material>() and entity.hasComponent<Transform>() and entity.hasComponent<Vao>())
			{
                cout << "Rendering entity: " << entity.name << endl;
				submit(
					entity.getComponent<Material>(), 
					*entity.getComponent<Transform>(), 
					*entity.getComponent<Vao>());
			}
		}
	}


	glfwSwapBuffers(window); // abstract mozda informacija ima u cherno video-ima: game engine window
	glfwPollEvents();

	endScene();
	
	return 0;
}

int RenderManager::shutDown() 
{
	
	delete[] voxelBuffer;
	delete sceneData;

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

void RenderManager::submit(Material* material, const Transform& transform, const Vao& vao)
{
    // PRE SETA MORA DA SHADER BUDE BOUNDOVAN
    material->shader.bind();

    material->set("u_Color", Vec4f(1, 0, 0, 1));

    material->set("u_Projection", sceneData->projectionMatrix);

    material->set("u_View", sceneData->viewMatrix);

    material->set("u_Model", transform);

    material->bind();
    // TODOOO -> material uniforme environment specific
    vao.bind();
    gRenderCommand.drawArrays(vao);
}
void RenderManager::beginScene(Entity* camera)
{
    if(!(camera->hasComponent<Cameraa>() and camera->hasComponent<PositionComponent>() and camera->hasComponent<RotationComponent>()))
    {
        cout << "ERROR: submitted entity is supposed to be a camera, but does NOT have required components" << endl;
        assert(false);
    }
    else {
        cout << "Camera entity: " << camera->name << endl;
    }

	camera->getComponent<Cameraa>()->recalculateViewMatrix(
        *camera->getComponent<PositionComponent>(),
        *camera->getComponent<RotationComponent>());


	sceneData->projectionMatrix = camera->getComponent<Cameraa>()->projectionMatrix;
	sceneData->viewMatrix = camera->getComponent<Cameraa>()->viewMatrix;
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