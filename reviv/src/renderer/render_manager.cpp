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
    RV_ASSERT(model.pMeshes.size() > 0 && model.pMaterials.size() > 0, "");
    RV_ASSERT(model.pMaterials.size() == model.pMeshes.size(), "");
    //RV_ASSERT(model.pMaterials.size() == model.pMeshes.size() || model.pMaterials.size() == 1, ""); // every mesh should have an equivalent material, OR there is just 1 material for every mesh

    for(unsigned int i=0; i<model.pMeshes.size(); i++)
    {
        model.pMaterials[i]->bind();
        bindEnvironment(*model.pMaterials[i]->pShader, transform);
        model.pMeshes[i]->vao.bind();

        RenderCommand::drawElements(*model.pMeshes[i]);
    }
}

void RenderManager::bindEnvironment(const Shader& shader, const Mat4& transform)
{
    shader.bind();
    
    shader.uploadUniformMat4("u_ModelMatrix", transform);
    shader.uploadUniformMat4("u_ViewMatrix", Scene::getCameraEntity()->get<CameraComponent>()->camera.viewMatrix);
    shader.uploadUniformMat4("u_ProjectionMatrix", Scene::getCameraEntity()->get<CameraComponent>()->camera.projectionMatrix);

    if(&shader != &AssetManager::get()->shaderPhong)
    {
        return;
    }

    int nrLight = 0;
    for(auto itEntity = Scene::getEntityList()->begin(); itEntity != Scene::getEntityList()->end(); itEntity++)
    {
        if(itEntity->valid)
        {
            if(itEntity->has<PointLightComponent>())
            {
                RV_ASSERT(nrLight < 10, ""); // can be implemented, just string and one digit stuff
                auto* pLight = &itEntity->get<PointLightComponent>()->light;

                if(pLight->on == false)
                {
                    continue;
                    RV_ASSERT(false, "");
                }

                std::string uniformNameLight = "u_PointLights[";
                uniformNameLight += (char)(nrLight + '0');
                uniformNameLight += "]";


                shader.uploadUniform3f(uniformNameLight + ".position", itEntity->get<TransformComponent>()->position);

                shader.uploadUniform3f(uniformNameLight + ".ambient", pLight->ambient);
                shader.uploadUniform3f(uniformNameLight + ".diffuse", pLight->diffuse);
                shader.uploadUniform3f(uniformNameLight + ".specular", pLight->specular);

                shader.uploadUniform1f(uniformNameLight + ".constant", pLight->constant);
                shader.uploadUniform1f(uniformNameLight + ".linear", pLight->linear);
                shader.uploadUniform1f(uniformNameLight + ".quadratic", pLight->quadratic);

                shader.uploadUniform3f("u_ViewPosition", Scene::getPlayerEntity()->get<TransformComponent>()->position);
            }
        }
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