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

    //deffered.init(Application::get()->getWindowWidth(), Application::get()->getWindowHeight());
    deffered.init(956, 1050);

    shadowMapShader.init("assets/shaders/shadow_map.vs", "assets/shaders/shadow_map.fs");

    shaderDefferedBlinnPhong.init("assets/shaders/deffered_blinn_phong.vs", "assets/shaders/deffered_blinn_phong.fs");
    materialDefferedBlinnPhong.setShader(&shaderDefferedBlinnPhong);
    materialDefferedBlinnPhong.setTexture("u_gPosition", deffered.gPosition);
    materialDefferedBlinnPhong.setTexture("u_gNormal", deffered.gNormal);
    materialDefferedBlinnPhong.setTexture("u_gAlbedoSpecular", deffered.gAlbedoSpecular);

    defaultFramebuffer.id = 0;
    defaultFramebuffer.isInited = true;

    shaderMonochroma.init("assets/shaders/monochroma.vs", "assets/shaders/monochroma.fs");
}

void RenderManager::iOnUpdate()
{
    renderSceneToFramebuffer(&defaultFramebuffer);
}

void RenderManager::renderSceneToFramebuffer(Framebuffer* pFrameBuffer)
{
    beginScene(); // Update camera stuff, update environment uniforms(doesn't upload them)
    //shadowMapRenderPass();
    defferedGeometryRenderPass();
    defferedLightingRenderPass();
    defferedMonochromaRenderPass();
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

    camera->setPerspectiveProjection(camera->m_HorizontalFov, Application::get()->getWindowRatio());
    if(Input::isKeyPressed(RV_KEY_P)) // TODO: remove
        camera->setOrthographicProjection(10, Application::get()->getWindowRatio());
    
    environment.set("ue_ViewMatrix", camera->viewMatrix);
    environment.set("ue_ProjectionMatrix", camera->projectionMatrix);
    
    environment.set("ue_ViewPosition", Scene::getCameraEntity()->get<TransformComponent>()->position);

    environment.setLights();
}

void RenderManager::shadowMapRenderPass()
{
    shadowMapShader.bind();

    for(auto itLight = Scene::getEntityList()->begin(); itLight != Scene::getEntityList()->end(); itLight++)
    {
        if(itLight->valid)
            continue;

        if(!(itLight->has<PointLightComponent>() || itLight->has<DirectionalLightComponent>()))
            continue;

        RV_ASSERT((itLight->has<PointLightComponent>() != itLight->has<DirectionalLightComponent>()) || (itLight->has<PointLightComponent>() == false), "entity can't have more than one light component on it"); // this is purely for mental simplification reasons

        ShadowMap* pShadowMap;
        Light* pLight;
        if(itLight->has<PointLightComponent>())
        {
            pLight = &itLight->get<PointLightComponent>()->light;
        }
        else if(itLight->has<DirectionalLightComponent>())
        {
            pLight = &itLight->get<DirectionalLightComponent>()->light;
        }
        else { RV_ASSERT(false, "light type not recognized"); }

        if(pLight->isShadowMapped == false || pLight->on == false)
            continue;

        pShadowMap = pLight->getShadowMap();

        pShadowMap->framebuffer.bind();
        glViewport(0, 0, pShadowMap->m_ResolutionWidth, pShadowMap->m_ResolutionHeight);
        glClear(GL_DEPTH_BUFFER_BIT);

        for(auto itOpaque = Scene::getEntityList()->begin(); itOpaque != Scene::getEntityList()->end(); itOpaque++)
        {
            if(itOpaque == itLight) // don't cast shadow on light that emits the light that casts the shadow
                continue;

            if(!itOpaque->valid)
                continue;

            if(itOpaque->has<ModelComponent>() && itOpaque->has<TransformComponent>())
            {
                Model* pModel = &itOpaque->get<ModelComponent>()->model;
                TransformComponent* pTransformComponent = itOpaque->get<TransformComponent>();

                shadowMapShader.uploadUniformMat4("u_ModelMatrix", pTransformComponent->getTransform());
                shadowMapShader.uploadUniformMat4("u_ShadowMapViewMatrix", pShadowMap->camera.viewMatrix);
                shadowMapShader.uploadUniformMat4("u_ShadowMapProjectionMatrix", pShadowMap->camera.projectionMatrix);

                for(unsigned int i=0; i < pModel->pMeshes.size(); i++)
                {
                    pModel->pMeshes[i]->vao.bind();
                    RenderCommand::drawElements(*pModel->pMeshes[i]);
                }
            }
        }
    }
}

void RenderManager::defferedGeometryRenderPass()
{
    deffered.gBuffer.bind();
    glViewport(0, 0, deffered.m_Width, deffered.m_Height);

    RenderCommand::setClearColor(Vec4f(0.f, 0.f, 0.f, 1.f));
    RenderCommand::clear();

    for(auto itEntity = Scene::getEntityList()->begin(); itEntity != Scene::getEntityList()->end(); itEntity++)
    {
        if(!itEntity->valid)
            continue;

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

void RenderManager::defferedLightingRenderPass()
{
        deffered.gBuffer.unbind();
        RV_ASSERT(Time::getLoopCounter() <= 1 || (Application::get()->getWindowWidth() == deffered.m_Width && Application::get()->getWindowHeight() == deffered.m_Height), "gBuffer for deffered rendering does not have the same width or height as the actual window");
        glViewport(0, 0, Application::get()->getWindowWidth(), Application::get()->getWindowHeight());
        bindEnvironmentAndMaterial(&shaderDefferedBlinnPhong, &environment, &materialDefferedBlinnPhong);

        RenderCommand::setClearColor(Vec4f(1.f, 0.f, 1.f, 1.f));
        RenderCommand::clear();
        AssetManager::get()->modelLoaderQuad2D.meshes[0].vao.bind();
        RenderCommand::drawElements(AssetManager::get()->modelLoaderQuad2D.meshes[0]);
}

void RenderManager::defferedMonochromaRenderPass()
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, deffered.gBuffer.id); //TODO: ovde se nesto verovatno moze ukloniti
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glViewport(0, 0, Application::get()->getWindowWidth(), Application::get()->getWindowHeight());
    int screenWidth = Application::get()->getWindow()->m_Data.width;
    int screenHeight = Application::get()->getWindow()->m_Data.height;
    glBlitFramebuffer(0, 0, screenWidth, screenHeight, 0, 0, screenWidth, screenHeight, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    for(stls::StableVector<Entity>::Iterator itEntity = Scene::getEntityList()->begin(); itEntity != Scene::getEntityList()->end(); itEntity++)
    {
        if(!itEntity->valid)
            continue;

        if(itEntity->has<ModelComponent>() && itEntity->has<TransformComponent>())
        {
            Model* pModel = &itEntity->get<ModelComponent>()->model;
            TransformComponent* pTransformComponent = itEntity->get<TransformComponent>();

            for(unsigned int i=0; i < pModel->pMeshes.size(); i++)
            {
                if(pModel->pMaterials[0]->pShader == &RenderManager::getInstance()->shaderMonochroma)
                {
                    //cout << "Rendering light: " << itEntity->entityName << endl;
                    bindEnvironmentAndMaterial(pModel->pMaterials[i]->pShader, &environment, pModel->pMaterials[i]);
                    pModel->pMaterials[i]->pShader->uploadUniformMat4("u_ModelMatrix", pTransformComponent->getTransform());

                    pModel->pMeshes[i]->vao.bind();

                    RenderCommand::drawElements(*pModel->pMeshes[i]);
                }
            }
        }
    }
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

void RenderManager::onEvent(Event* event)
{
    RV_ASSERT(event->m_Type == EventTypeWindowResize, "event not recognized");
    if(event->m_Type == EventTypeWindowResize)
    {
        EventWindowResize* resizeEvent = (EventWindowResize*)event;
        getInstance()->deffered.resize(resizeEvent->m_Width, resizeEvent->m_Height);
    }

}

void RenderManager::iShutdown() 
{
}
