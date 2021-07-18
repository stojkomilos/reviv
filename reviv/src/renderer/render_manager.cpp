#include"render_manager.h"

void RenderManager::iOnUpdate(const WindowData& windowData)
{
    RenderCommand::setClearColor(Vec4f(0.1f, 0.0f, 0.0f, 0.8f));
    RenderCommand::clear();

    beginScene(windowData); // Update camera stuff

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

void RenderManager::iInit(const WindowData& windowData)
{
    RenderCommand::init();

    auto* cameraEntity = Scene::getCameraEntity();
    cameraEntity->get<CameraComponent>()->camera.recalculateViewMatrix(cameraEntity->get<PositionComponent>()->position, cameraEntity->get<RotationComponent>()->rotation);
    cameraEntity->get<CameraComponent>()->camera.recalculateProjectionMatrix(windowData);
}

void RenderManager::submit(Material* pMaterial, const Mat4& transform, const Vao& vao)
{
    pMaterial->bind();
    vao.bind();
    RenderCommand::drawArrays(vao);
}

void RenderManager::iShutdown() 
{
	delete[] voxelBuffer;
}

void RenderManager::beginScene(const WindowData& windowData)
{
    Camera* camera = &Scene::getCameraEntity()->get<CameraComponent>()->camera;

    RV_ASSERT(Scene::getCameraEntity()->has<CameraComponent>()
        and Scene::getCameraEntity()->has<PositionComponent>()
        and Scene::getCameraEntity()->has<RotationComponent>(),
        "submitted entity is supposed to be a camera, but does NOT have required components");

    camera->recalculateViewMatrix(
        Scene::getCameraEntity()->get<PositionComponent>()->position,
        Scene::getCameraEntity()->get<RotationComponent>()->rotation);

    camera->recalculateProjectionMatrix(windowData);

}

void RenderManager::endScene()
{

}