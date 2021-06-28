#include"reviv/renderer/render_manager.h"
#include"reviv/renderer/opengl_assert.h"

#ifdef NDEBUG
#define __REVIV_RELEASE__ 1
#else
#define __REVIV_RELEASE__ 0
#endif

extern SimulationManager gSimulationManager;
extern ModelLoader sphere;
extern ModelLoader cube;
extern mat::mat4 identity;
extern PhysicsManager gPhysicsManager;
extern int gGameLoopCounter;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

RenderCommand gRenderCommand;
int RenderManager::startUp(int windowWidth1, int windowHeight1) {

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
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	
	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;

	GLCall(glViewport(0, 0, windowWidth, windowHeight));
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE); ///pale se sticky mouse 
	glfwSetCursorPosCallback(window, mouse_callback);

	shaderTexture.setUp("res/shaders/texture.vs", "res/shaders/texture.fs");
	shaderMonoChroma.setUp("res/shaders/mono_chroma.vs", "res/shaders/mono_chroma.fs");  
	
	std::vector<BufferElement> tempVboLayout1 = {
		{ShaderDataType::Float3, "a_Position", false},
		{ShaderDataType::Float2, "a_TexCoord", false},
		{ShaderDataType::Float3, "a_Normal",   false},
	};
	BufferLayout vboLayout1(tempVboLayout1);

	sphereVao.setUp();
	sphereVao.bind();

	sphereVbo.layout = vboLayout1;
	sphereVbo.setUp(sphere.pointer, sphere.nrTriangles * 3 * (2 * sizeof(mat::vec3) + sizeof(mat::vec2)), 0);	
	sphereVao.addVertexBuffer(sphereVbo);

	assert(sizeof(float) * 3 == sizeof(mat::vec3));
	assert(sizeof(int) * 3 == sizeof(TripletOfInts));


	nrOfValidCubes = 0;
	for (int i = 0; i < gPhysicsManager.numberOfTerrainCubesOnStart; i++) {
		if ((gPhysicsManager.kocke + i)->valid)
			nrOfValidCubes++;
	}
	
	//sizeOfVoxelBuffer = sizeof(float) * 3 * (nrOfValidCubes + 100) + sizeof(cubeVertices);
	//voxelBuffer = new float[sizeOfVoxelBuffer / sizeof(float)];
	//
	//memcpy(voxelBuffer, &cubeVertices[0], sizeof(cubeVertices));
	//
	//int p = 0;
	//for (int i = 0; i < gPhysicsManager.numberOfTerrainCubesOnStart; i++) {
	//	if ((gPhysicsManager.kocke + i)->valid) {
	//		voxelBuffer[sizeof(cubeVertices) / sizeof(float) + p * 3 + 0] = (gPhysicsManager.kocke + i)->position.x;
	//		voxelBuffer[sizeof(cubeVertices) / sizeof(float) + p * 3 + 1] = (gPhysicsManager.kocke + i)->position.y;
	//		voxelBuffer[sizeof(cubeVertices) / sizeof(float) + p * 3 + 2] = (gPhysicsManager.kocke + i)->position.z;
	//		p++;
	//	}
	//}
	
	int nrAttributes;											 //samo jedna provere dal ima otprilike dovoljno atrib pointera
	GLCall(glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes));
	std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;
	assert(nrAttributes >= 10); 


	cubeVao.setUp();
	cubeVao.bind();
	
	cubeVbo.layout = vboLayout1;
	cubeVbo.setUp((void*)cube.pointer, cube.nrTriangles * 3 * (2 * sizeof(mat::vec3) + sizeof(mat::vec2)), 0);
	cubeVbo.bind();
	cubeVao.addVertexBuffer(cubeVbo);

	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	stanicTexture.setUp("res/textures/stene.png");
	beloTexture.setUp("res/textures/belo.png");

	//---
	sceneData = new SceneData;
	camera.setUp(0.1f, renderDistance, 60.0f / 180.0f * 2.0f * 3.14f, ((float)(windowWidth)) / ((float)(windowHeight)));
	camera.recalculateProjectionMatrix();
	
	///--

	return 1;
}

int RenderManager::render() {
	
	mat::mat4 model;


	gRenderCommand.setClearColor(mat::vec4(0.0f, 0.0f, 0.0f, 0.0f));
	gRenderCommand.clear();

	camera.alignWithEntity(gPhysicsManager.player);
	beginScene(camera);

	extern mat::vec3 gPointLightPosition;

	for (int i = 0; i < gPhysicsManager.numberOfTerrainCubesOnStart; i++) {
		if ((gPhysicsManager.kocke + i)->valid) {

			shaderTexture.bind();
			
			model = mat::translate(identity, (gPhysicsManager.kocke + i)->position);
			shaderTexture.uploadUniform3f("u_LightPosition", gPointLightPosition);
			shaderTexture.uploadUniform3f("u_CameraPosition", camera.position);
			shaderTexture.uploadUniform1i("u_Texture", 1);
			beloTexture.bind(1);
			submit(shaderTexture, cubeVao, model);
		}
	}

	
	model = mat::translate(identity, gPointLightPosition);
	shaderMonoChroma.bind();
	shaderMonoChroma.uploadUniform4f("u_Color", mat::vec4(1, 1, 1, 1));

	stanicTexture.bind(0);
	shaderMonoChroma.uploadUniform1i("u_Texture", 0);

	submit(shaderMonoChroma, cubeVao, model);


	//ovo moze da se apdejtuje mnogo redje
	//int p = 0;
	//for (int i = 0; i < gPhysicsManager.numberOfTerrainCubesOnStart; i++) {
	//	if ((gPhysicsManager.kocke + i)->valid) {
	//		voxelBuffer[sizeof(cubeVertices) / sizeof(float) + p * 3 + 0] = (gPhysicsManager.kocke + i)->position.x;
	//		voxelBuffer[sizeof(cubeVertices) / sizeof(float) + p * 3 + 1] = (gPhysicsManager.kocke + i)->position.y;
	//		voxelBuffer[sizeof(cubeVertices) / sizeof(float) + p * 3 + 2] = (gPhysicsManager.kocke + i)->position.z;
	//		p++;
	//	}



	glfwSwapBuffers(window);
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

void RenderManager::submit(Shader& shader, VertexArray& object, mat::mat4& transform) {
	shader.bind();
	shader.uploadUniformMat4("u_Model", transform);
	shader.uploadUniformMat4("u_View", sceneData->viewMatrix);
	shader.uploadUniformMat4("u_Projection", sceneData->projectionMatrix);
	object.bind();
	gRenderCommand.drawArrays(object);
}

void RenderManager::beginScene(PerspectiveCamera& camera)
{
	sceneData->projectionMatrix = camera.projectionMatrix;
	sceneData->viewMatrix = camera.viewMatrix;
}
void RenderManager::endScene()
{

}
