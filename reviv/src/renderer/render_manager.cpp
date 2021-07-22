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
            //if(itEntity->has<MaterialComponent>() and itEntity->has<TransformComponent>() and itEntity->has<ModelComponent>())
            if(itEntity->has<ModelComponent>() and itEntity->has<TransformComponent>())
            {
                cout << "Rendering entity: " << itEntity->entityName << endl;

                submit(itEntity->get<ModelComponent>()->model,
                    itEntity->get<TransformComponent>()->transform);

                //submit(
                //    &itEntity->get<MaterialComponent>()->material, 
                //    itEntity->get<TransformComponent>()->transform, 
                //    itEntity->get<ModelComponent>()->model);

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

void RenderManager::submit(const Model& model, const Mat4& transform)
{
    RV_ASSERT(model.pMaterials.size() == model.pMeshes.size(), "");
    for(unsigned int i=0; i<model.pMeshes.size(); i++)
    {
        //TODO: set environment uniforms
        model.pMaterials[i]->bindShader();
        model.pMaterials[i]->set("m_Model", transform);
        model.pMaterials[i]->bind();

        model.pMeshes[i]->vao.bind();

        RenderCommand::drawElements(*model.pMeshes[i]);
    }
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