#include"render_manager.h"

void RenderManager::iInit()
{
    RenderCommand::init();

    std::vector<std::string> skyboxFaces;
    skyboxFaces.push_back("assets/textures/skybox/right.jpg"); // the order of theese makes no sense within the context of the world coordinate system, but it is used because opengl incorectly rotates the cube map textures within this coordinate system
    skyboxFaces.push_back("assets/textures/skybox/left.jpg");
    skyboxFaces.push_back("assets/textures/skybox/bottom.jpg");
    skyboxFaces.push_back("assets/textures/skybox/top.jpg");
    skyboxFaces.push_back("assets/textures/skybox/front.jpg");
    skyboxFaces.push_back("assets/textures/skybox/back.jpg");
    skybox.init(skyboxFaces);

    deffered.init();

    shadowMapShader.init("assets/shaders/shadow_map.vs", "assets/shaders/shadow_map.fs");

    shaderDefferedBlinnPhong.init("assets/shaders/deffered_blinn_phong.vs", "assets/shaders/deffered_blinn_phong.fs");
    materialDefferedBlinnPhong.setShader(&shaderDefferedBlinnPhong);
    materialDefferedBlinnPhong.setTexture("u_gPosition", deffered.gPosition);
    materialDefferedBlinnPhong.setTexture("u_gNormal", deffered.gNormal);
    materialDefferedBlinnPhong.setTexture("u_gAlbedoSpecular", deffered.gAlbedoSpecular);
}

void RenderManager::iOnUpdate()
{
    beginScene(); // Update camera stuff, update environment uniforms(doesn't upload them)

    for(auto itLight = Scene::getEntityList()->begin(); itLight != Scene::getEntityList()->end(); itLight++)
    {
        if(itLight->valid)
        {

            if(itLight->has<PointLightComponent>() || itLight->has<DirectionalLightComponent>())
            {
                RV_ASSERT((itLight->has<PointLightComponent>() != itLight->has<DirectionalLightComponent>()) || (itLight->has<PointLightComponent>() == false), "entity can't have more than one light component on it"); // this is purely for mental simplification reasons

                ShadowMap* pShadowMap;
                if(itLight->has<PointLightComponent>())
                {
                    RV_ASSERT(false, ""); // temporary, can be removed
                }
                else if(itLight->has<DirectionalLightComponent>())
                {
                    DirectionalLight* pDirectionalLight = &itLight->get<DirectionalLightComponent>()->light;

                    if(pDirectionalLight->isShadowMapped == false || pDirectionalLight->on == false)
                        continue;

                    pShadowMap = &pDirectionalLight->shadowMap;
                }

                pShadowMap->framebuffer.bind();
                glClear(GL_DEPTH_BUFFER_BIT);
                glViewport(0, 0, pShadowMap->m_ResolutionWidth, pShadowMap->m_ResolutionHeight);

                for(auto itOpaque = Scene::getEntityList()->begin(); itOpaque != Scene::getEntityList()->end(); itOpaque++)
                {
                    if(itOpaque == itLight) // don't cast shadow on yourself
                        continue;


                    if(itOpaque->has<ModelComponent>() && itOpaque->has<TransformComponent>())
                    {
                        cout << "Shadow by light: " << itLight->entityName << " for entity: " << itOpaque->entityName << endl;
                        Model* pModel = &itOpaque->get<ModelComponent>()->model;
                        TransformComponent* pTransformComponent = itOpaque->get<TransformComponent>();

                        for(unsigned int i=0; i < pModel->pMeshes.size(); i++)
                        {
                            bindEnvironment(&shadowMapShader, &environment); // TODO: unnecessary to call every time, can be greatly reduced maybe?
                            shadowMapShader.uploadUniformMat4("u_ModelMatrix", pTransformComponent->getTransform());
                            shadowMapShader.uploadUniformMat4("u_ShadowMapViewProjectionMatrix", pTransformComponent->getTransform());
                            pModel->pMeshes[i]->vao.bind();

                            RenderCommand::drawElements(*pModel->pMeshes[i]);
                        }
                    }
                }

            }
        }
    }

    deffered.gBuffer.bind();

    RenderCommand::setClearColor(Vec4f(0.f, 0.f, 0.f, 1.f));
    RenderCommand::clear();

    for(auto itEntity = Scene::getEntityList()->begin(); itEntity != Scene::getEntityList()->end(); itEntity++)
    {
        if(itEntity->valid)
        {
            if(itEntity->has<ModelComponent>() && itEntity->has<TransformComponent>())
            {
                Model* pModel = &itEntity->get<ModelComponent>()->model;
                TransformComponent* pTransformComponent = itEntity->get<TransformComponent>();

                for(unsigned int i=0; i < pModel->pMeshes.size(); i++)
                {
                    if(pModel->pMaterials[0]->pShader == &deffered.geometryPassShader)
                    {
                        bindEnvironmentAndMaterial(pModel->pMaterials[i]->pShader, &environment, pModel->pMaterials[i]);
                        pModel->pMaterials[i]->pShader->uploadUniformMat4("u_ModelMatrix", pTransformComponent->getTransform());

                        pModel->pMeshes[i]->vao.bind();

                        RenderCommand::drawElements(*pModel->pMeshes[i]);
                    }
                }
            }
        }
    }
    deffered.gBuffer.unbind();

    bindEnvironmentAndMaterial(&shaderDefferedBlinnPhong, &environment, &materialDefferedBlinnPhong);

    RenderCommand::setClearColor(Vec4f(1.f, 0.f, 1.f, 1.f));
    RenderCommand::clear();
    AssetManager::get()->modelLoaderQuad2D.meshes[0].vao.bind();
    RenderCommand::drawElements(AssetManager::get()->modelLoaderQuad2D.meshes[0]);

    glBindFramebuffer(GL_READ_FRAMEBUFFER, deffered.gBuffer.id); //TODO: ovde se nesto verovatno moze ukloniti
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    int screenWidth = Application::get()->getWindow()->m_Data.width;
    int screenHeight = Application::get()->getWindow()->m_Data.height;
    glBlitFramebuffer(0, 0, screenWidth, screenHeight, 0, 0, screenWidth, screenHeight, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    for(stls::StableVector<Entity>::Iterator itEntity = Scene::getEntityList()->begin(); itEntity != Scene::getEntityList()->end(); itEntity++)
    {
        if(itEntity->valid)
        {
            if(itEntity->has<ModelComponent>() && itEntity->has<TransformComponent>())
            {
                Model* pModel = &itEntity->get<ModelComponent>()->model;
                TransformComponent* pTransformComponent = itEntity->get<TransformComponent>();

                for(unsigned int i=0; i < pModel->pMeshes.size(); i++)
                {
                    if(pModel->pMaterials[0]->pShader == &AssetManager::get()->shaderMonochroma)
                    {
                        cout << "Rendering light: " << itEntity->entityName << endl;

                        bindEnvironmentAndMaterial(pModel->pMaterials[i]->pShader, &environment, pModel->pMaterials[i]);
                        pModel->pMaterials[i]->pShader->uploadUniformMat4("u_ModelMatrix", pTransformComponent->getTransform());

                        pModel->pMeshes[i]->vao.bind();

                        RenderCommand::drawElements(*pModel->pMeshes[i]);
                    }
                }
            }
        }
    }

    skybox.onUpdate();
}

void RenderManager::beginScene()
{
    Camera* camera = &Scene::getCameraEntity()->get<CameraComponent>()->camera;

    RV_ASSERT(Scene::getCameraEntity()->has<CameraComponent>()
        && Scene::getCameraEntity()->has<TransformComponent>(),
        "submitted entity is supposed to be a camera, but does NOT have required components");

    camera->setViewMatrix(
        Scene::getCameraEntity()->get<TransformComponent>()->position,
        Scene::getCameraEntity()->get<TransformComponent>()->rotation);

    camera->setPerspectiveProjection();

    //Scene::getCameraEntity()->get<CameraComponent>()->camera.viewMatrix = lightCamera.viewMatrix;
    //Scene::getCameraEntity()->get<CameraComponent>()->camera.projectionMatrix = lightCamera.projectionMatrix;

    environment.set("ue_ViewMatrix", camera->viewMatrix);
    environment.set("ue_ProjectionMatrix", camera->projectionMatrix);
    
    environment.set("ue_ViewPosition", Scene::getCameraEntity()->get<TransformComponent>()->position);

    environment.setLights();
}

void RenderManager::iShutdown() 
{
}

void RenderManager::bindEnvironmentAndMaterial(Shader* shader, Environment* environment, Material* material)
{
    RV_ASSERT(shader == material->pShader, "specified shader is not the the same shader that the material owns");
    shader->bind();
    environment->bind(shader); // MUST bind environment before material
    material->bind();
}

void RenderManager::bindEnvironment(Shader* shader, Environment* environment)
{
    shader->bind();
    environment->bind(shader);
}