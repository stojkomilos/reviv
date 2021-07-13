#include"render_manager.h"

extern int gGameLoopCounter;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

extern Entity* stanic;
extern Entity* player;


RenderCommand gRenderCommand;
void RenderManager::init()
{

    window.init();

    /// -------- MATERIALS ------------------------------------------------------------

	praviMono.setUp("../resources/shaders/monocolor.vs", "../resources/shaders/monocolor.fs");
    Shader* textureShader = new Shader; // TODO, memory leak
	textureShader->setUp("../resources/shaders/texture.vs", "../resources/shaders/texture.fs");
    auto* stanicMat = stanic->add<MaterialComponent>();
    //material->
    //stanicMat->material.set("u_Color", Vec4f(1, 0, 0, 1));
    *stanicMat = MaterialComponent(Material(*textureShader));
    stanicMat->material.set("u_Color", Vec4f(1, 0, 0, 1));

    /// -----
	
	std::vector<BufferElement> tempVboLayout1 = {
		{ShaderDataType::SdtFloat3, "a_Position", false},
		{ShaderDataType::SdtFloat2, "a_TexCoord", false},
		{ShaderDataType::SdtFloat3, "a_Normal",   false},
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

    gRenderCommand.init();


	stanicTexture.setUp("../resources/textures/stene.png");
    stanicTexture.bind(0);
	//beloTexture.setUp("../resources/textures/belo.png");

	Scene::getCameraEntity()->get<CameraComponent>()->camera.setUp(0.1f, renderDistance, 60.0f / 180.0f * 2.0f * 3.14f, ((float)(window.getWidth())) / ((float)(window.getHeight())));
	Scene::getCameraEntity()->get<CameraComponent>()->camera.recalculateProjectionMatrix();
}

void RenderManager::render()
{
	gRenderCommand.setClearColor(Vec4f(0.0f, 0.0f, 0.0f, 0.9f));
	gRenderCommand.clear();

	beginScene();

    for(auto itEntity = Scene::getEntityList()->begin(); itEntity != Scene::getEntityList()->end(); itEntity++)
    {
		if(itEntity->valid)
		{
			if(itEntity->has<MaterialComponent>() and itEntity->has<TransformComponent>() and itEntity->has<VaoComponent>())
			{
            //    stanic->get<MaterialComponent>()->material.set("u_Color", Vec4f(1, 0, 0, 1));
                //cout << "Rendering entity: " << itEntity->entityName << endl;
				submit(
					&itEntity->get<MaterialComponent>()->material, 
					itEntity->get<TransformComponent>()->transform, 
					itEntity->get<VaoComponent>()->vao);
			}
		}
	}

	endScene();
}

void RenderManager::submit(Material* material, const Mat4& transform, const Vao& vao)
{
    // PRE SETA MORA DA SHADER BUDE BOUNDOVAN
    //material->shader.bind();

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

void RenderManager::shutdown() 
{
	delete[] voxelBuffer;
}

void RenderManager::beginScene()
{
    Camera* camera = &Scene::getCameraEntity()->get<CameraComponent>()->camera;

    assert(Scene::getCameraEntity()->has<CameraComponent>() // ERROR: submitted entity is supposed to be a camera, but does NOT have required components
        and Scene::getCameraEntity()->has<PositionComponent>()
        and Scene::getCameraEntity()->has<RotationComponent>());

	camera->recalculateViewMatrix(
        *Scene::getCameraEntity()->get<PositionComponent>(),
        *Scene::getCameraEntity()->get<RotationComponent>());

}
void RenderManager::endScene()
{

}