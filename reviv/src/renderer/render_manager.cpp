#include"render_manager.h"

void RenderManager::iOnUpdate(const WindowData& windowData)
{
    RenderCommand::setClearColor(Vec4f(100.f, 10.f, 80.f, 256.f) / 256.f);
    RenderCommand::clear();

    beginScene(windowData); // Update camera stuff

    for(auto itEntity = Scene::getEntityList()->begin(); itEntity != Scene::getEntityList()->end(); itEntity++)
    {
        if(itEntity->valid)
        {
            if(itEntity->has<ModelComponent>() && itEntity->has<TransformComponent>())
            {
                //cout << "Rendering entity: " << itEntity->entityName << endl;
                submit(itEntity->get<ModelComponent>()->model,
                    itEntity->get<TransformComponent>()->getTransform());
            }
        }
    }
    endScene();
}

void RenderManager::iInit(const WindowData& windowData)
{
    RenderCommand::init();

    auto* cameraEntity = Scene::getCameraEntity();
    cameraEntity->get<CameraComponent>()->camera.recalculateViewMatrix(cameraEntity->get<TransformComponent>()->position, cameraEntity->get<TransformComponent>()->rotation);
    cameraEntity->get<CameraComponent>()->camera.recalculateProjectionMatrix(windowData);
}

void RenderManager::submit(const Model& model, const Mat4& transform)
{
    RV_ASSERT(model.pMaterials.size() == model.pMeshes.size(), "");
    for(unsigned int i=0; i<model.pMeshes.size(); i++)
    {
        model.pMaterials[i]->set("u_Model", transform);
        model.pMaterials[i]->set("u_View", Scene::getCameraEntity()->get<CameraComponent>()->camera.viewMatrix);
        model.pMaterials[i]->set("u_Projection", Scene::getCameraEntity()->get<CameraComponent>()->camera.projectionMatrix);

        model.pMaterials[i]->bind();

        model.pMeshes[i]->vao.bind();

        RenderCommand::drawElements(*model.pMeshes[i]);
    }
}

void RenderManager::iShutdown() 
{
}

void RenderManager::beginScene(const WindowData& windowData)
{
    Camera* camera = &Scene::getCameraEntity()->get<CameraComponent>()->camera;

    RV_ASSERT(Scene::getCameraEntity()->has<CameraComponent>()
        && Scene::getCameraEntity()->has<TransformComponent>(),
        "submitted entity is supposed to be a camera, but does NOT have required components");

    camera->recalculateViewMatrix(
        Scene::getCameraEntity()->get<TransformComponent>()->position,
        Scene::getCameraEntity()->get<TransformComponent>()->rotation);

    camera->recalculateProjectionMatrix(windowData);

}

void RenderManager::endScene()
{

}