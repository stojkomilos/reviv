#include"render_manager.h"

extern int gGameLoopCounter;
void framebuffer_size_callback(GLFWwindow* window, int width, int height); //TODO:
void mouse_callback(GLFWwindow* window, double xpos, double ypos); // TODO:

void RenderManager::iOnUpdate()
{
    RenderCommand::setClearColor(Vec4f(0.1f, 0.0f, 0.0f, 0.8f));
	RenderCommand::clear();

	beginScene();

    for(auto itEntity = Scene::getEntityList()->begin(); itEntity != Scene::getEntityList()->end(); itEntity++)
    {
		if(itEntity->valid)
		{
			if(itEntity->has<MaterialComponent>() and itEntity->has<TransformComponent>() and itEntity->has<VaoComponent>())
			{
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

void RenderManager::iInit()
{
    RenderCommand::init();

    /// -------- MATERIALS ------------------------------------------------------------
    

    /// -----


    ///
    //auto* stanicVao = &stanic->add<VaoComponent>()->vao;
    //auto* stanicModel = &stanic->get<ModelComponent>()->Model;
    //stanicVao->setUp();
    //stanicVao->bind();

    //stanicVao->vbo.layout = vboLayout1;
    //stanicVao->vbo.setUp((void*)stanicModel->pointer, stanicModel->nrTriangles * 3 * (2 * sizeof(Vec3f) + sizeof(Vec2f)), 0);
    //stanicVao->vbo.bind();
    //stanicVao->addVertexBuffer(stanic->get<VaoComponent>()->vao.vbo);
    ///

    modelCube.init("../../sandbox/assets/models/cube.obj");
    shaderTexture.init("../../sandbox/assets/shaders/texture.vs", "../../sandbox/assets/shaders/texture.fs");
    //shaderMonochroma.init("../sandbox/assets/shaders/monochroma.vs", "../sandbox/assets/shaders/monochroma.fs");

    //Scene::getCameraEntity()->get<CameraComponent>()->camera.setUp(0.1f, renderDistance, 60.0f / 180.0f * 2.0f * 3.14f, ((float)(window.getWidth())) / ((float)(window.getHeight())));
    Scene::getCameraEntity()->get<CameraComponent>()->camera.setUp(0.1f, renderDistance, 60.0f / 180.0f * 2.0f * 3.14f, ((float)(16.0/9.0)));
    Scene::getCameraEntity()->get<CameraComponent>()->camera.recalculateProjectionMatrix();
}


void RenderManager::submit(Material* pMaterial, const Mat4& transform, const Vao& vao)
{
    // PRE SETA MORA DA SHADER BUDE BOUNDOVAN
    pMaterial->pShader->bind();

    pMaterial->set("u_Color", Vec4f(0, 0, 1, 1));

    pMaterial->set("u_Projection", Scene::getCameraEntity()->get<CameraComponent>()->camera.projectionMatrix);

    pMaterial->set("u_View", Scene::getCameraEntity()->get<CameraComponent>()->camera.viewMatrix);

    pMaterial->set("u_Model", transform);

    pMaterial->set("u_Texture", 0);

    pMaterial->bind();
    // TODOOO -> material uniforme environment specific

    vao.bind();
    RenderCommand::drawArrays(vao);
}

void RenderManager::iShutdown() 
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
        Scene::getCameraEntity()->get<PositionComponent>()->position,
        Scene::getCameraEntity()->get<RotationComponent>()->rotation);

}

void RenderManager::endScene()
{

}