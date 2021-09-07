#include"render_manager.h"

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

    deffered.init(Application::get()->getWindowWidth(), Application::get()->getWindowHeight());

    //directionalShadowMapShader.init("assets/shaders/shadow_map.vs", "assets/shaders/shadow_map.fs");
    //omnidirectionalShadowMapShader.init("assets/shaders/omnidirectional_shadow_map_6times.vs", "assets/shaders/omnidirectional_shadow_map_6times.fs");

    shaderDefferedLighting.init("assets/shaders/deffered_blinn_phong.vs", "assets/shaders/deffered_blinn_phong.fs");
    materialDefferedLighting.setShader(&shaderDefferedLighting);
    materialDefferedLighting.setTexture("u_gPosition", deffered.gPosition);
    materialDefferedLighting.setTexture("u_gNormal", deffered.gNormal);
    materialDefferedLighting.setTexture("u_gAlbedoSpecular", deffered.gAlbedoSpecular);

    defaultFramebuffer.id = 0;
    defaultFramebuffer.isInited = true;

    shaderMonochroma.init("assets/shaders/monochroma.vs", "assets/shaders/monochroma.fs");
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
    camera->setPerspectiveProjection(degreesToRadians(90), Application::get()->getWindowRatio());

    environment.set("ue_ViewMatrix", camera->viewMatrix);
    environment.set("ue_ProjectionMatrix", camera->projectionMatrix);
    
    environment.set("ue_ViewPosition", Scene::getCameraEntity()->get<TransformComponent>()->position);

    environment.setLights();
}

/*
void RenderManager::shadowMapRenderPass()
{

    for(auto itLight = Scene::getEntityList()->begin(); itLight != Scene::getEntityList()->end(); itLight++)
    {
        if(!itLight->valid)
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

        if(pLight->lightType == LightType::LightTypeDirectional)
        {
            directionalShadowMapShader.bind();
        }
        else {
            RV_ASSERT(pLight->lightType == LightType::LightTypePoint, "unrecognized light type");
            omnidirectionalShadowMapShader.bind();
        }

        pShadowMap = pLight->getShadowMap();

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
                //cout << "ShadowMapping light: " << itLight->entityName << " opaque: " << itOpaque->entityName << endl;
                Model* pModel = &itOpaque->get<ModelComponent>()->model;
                TransformComponent* pTransformComponent = itOpaque->get<TransformComponent>();

                if(pLight->lightType == LightType::LightTypeDirectional)
                {
                    directionalShadowMapShader.uploadUniformMat4("u_ModelMatrix", pTransformComponent->getTransform());
                    directionalShadowMapShader.uploadUniformMat4("u_ShadowMapViewMatrix", pShadowMap->camera.viewMatrix);
                    directionalShadowMapShader.uploadUniformMat4("u_ShadowMapProjectionMatrix", pShadowMap->camera.projectionMatrix);

                    for(unsigned int i=0; i < pModel->pMeshes.size(); i++)
                    {
                        pModel->pMeshes[i]->vao.bind();
                        RenderCommand::drawElements(*pModel->pMeshes[i]);
                    }
                }
                else {
                    // TODO: ovaj define
                    #define RV_FLT_MAX 30000000000000000.f


                    glClearColor(RV_FLT_MAX, RV_FLT_MAX, RV_FLT_MAX, RV_FLT_MAX);
                    omnidirectionalShadowMapShader.uploadUniformMat4("u_ModelMatrix", pTransformComponent->getTransform());
                    omnidirectionalShadowMapShader.uploadUniform3f("u_LightPosition", itLight->get<TransformComponent>()->position);
                    omnidirectionalShadowMapShader.uploadUniform1f("u_FarPlane", pLight->getShadowMap()->renderDistance);
                    
                    Camera tempCamera;
                    tempCamera.renderDistance = pLight->getShadowMap()->renderDistance;
                    tempCamera.nearRenderDistance = pLight->getShadowMap()->nearRenderDistance;
                    tempCamera.setPerspectiveProjection(degreesToRadians(90), 1.f);
                    omnidirectionalShadowMapShader.uploadUniformMat4("u_ShadowProjectionMatrix", tempCamera.projectionMatrix);

                    Vec3f lightPos = itLight->get<TransformComponent>()->position;
                    std::vector<Vec3f> shadowDirections;

                    if(1) // moj
                    {
                        shadowDirections.push_back(Vec3f(0, -1, 0));
                        shadowDirections.push_back(Vec3f(0, 1, 0));
                        shadowDirections.push_back(Vec3f(0, 0, -1));
                        shadowDirections.push_back(Vec3f(0, 0, 1));
                        shadowDirections.push_back(Vec3f(1, 0, 0));
                        shadowDirections.push_back(Vec3f(-1, 0, 0));
                    }
                    else {
                        shadowDirections.push_back(Vec3f(1, 0, 0));
                        shadowDirections.push_back(Vec3f(-1, 0, 0));
                        shadowDirections.push_back(Vec3f(0, 1.0, 0));
                        shadowDirections.push_back(Vec3f(0, -1.0, 0));
                        shadowDirections.push_back(Vec3f(0, 0, 1.0));
                        shadowDirections.push_back(Vec3f(0, 0, -1.0));
                    }

                    for(int face=0; face<6; face++)
                    {
                        
                        pShadowMap->framebuffer.bind();
                        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 
                            GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, ((ShadowMapOmnidirectional*)pShadowMap)->shadowMap.id, 0);
                        glDrawBuffer(GL_COLOR_ATTACHMENT0);
                        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

                        std::string baseString = "u_ShadowViewMatrix";
                        tempCamera.setViewMatrix(lightPos, lookAtGetRotation(lightPos, lightPos + shadowDirections[face]));
                        for(unsigned int i=0; i < pModel->pMeshes.size(); i++)
                        {
                            pModel->pMeshes[i]->vao.bind();
                            RenderCommand::drawElements(*pModel->pMeshes[i]);
                        }
                    }
                }
            }
        }
    }
}
*/

//righ
//left
//bott
//top.
//fron
//back

void RenderManager::defferedGeometryRenderPass()
{
    deffered.gBuffer.bind();
    glViewport(0, 0, deffered.m_Width, deffered.m_Height);
    RV_ASSERT(Time::getLoopCounter() <= 1 || (Application::get()->getWindowWidth() == deffered.m_Width && Application::get()->getWindowHeight() == deffered.m_Height), "gBuffer for deffered rendering does not have the same width or height as the actual window");

    RenderCommand::setClearColor(Vec4f(0.f, 0.f, 0.f, 1.f));
    RenderCommand::clear();

    log(environment);

    for(auto itEntity = Scene::getEntityList()->begin(); itEntity != Scene::getEntityList()->end(); itEntity++)
    {
        if(!itEntity->valid)
            continue;

        if(itEntity->has<ModelComponent>() && itEntity->has<TransformComponent>())
        {
            Model* pModel = &itEntity->get<ModelComponent>()->model;
            TransformComponent* pTransformComponent = itEntity->get<TransformComponent>();

            glDisable(GL_CULL_FACE);
            // TODO:
            //if(pModel->flags.isCullFaceOn == true)
            //    glEnable(GL_CULL_FACE);
            //else glDisable(GL_CULL_FACE);

            for(unsigned int i=0; i < pModel->pMeshes.size(); i++)
            {
                if(pModel->pMaterials[i]->pShader == &deffered.geometryPassShader)
                {
                    //cout << "---" << endl;
                    //cout << "Geometry deffered pass for entity: " << itEntity->entityName << " . modelMatrix: ";
                    log(pTransformComponent->getTransform());
                    bindEnvironmentAndMaterial(&deffered.geometryPassShader, &environment, pModel->pMaterials[i]);
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
        glViewport(0, 0, Application::get()->getWindowWidth(), Application::get()->getWindowHeight());
        bindEnvironmentAndMaterial(&shaderDefferedLighting, &environment, &materialDefferedLighting);

        glDisable(GL_CULL_FACE);

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
                if(pModel->pMaterials[i]->pShader == &RenderManager::get()->shaderMonochroma)
                {
                    //cout << "Rendering light: " << itEntity->entityName << endl;
                    bindEnvironmentAndMaterial(&RenderManager::get()->shaderMonochroma, &environment, pModel->pMaterials[i]);
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
    shader->textureUniformCounter = 0;
    RV_ASSERT(shader == material->pShader, "specified shader is not the the same shader that the material owns");
    shader->bind();
    environment->bind(shader); // MUST bind environment before material
    material->bind();
}

void RenderManager::bindEnvironment(Shader* shader, Environment* environment)
{
    shader->textureUniformCounter = 0;
    shader->bind();
    environment->bind(shader);
}

void RenderManager::bindMaterial(Shader* shader, Material* material)
{
    shader->textureUniformCounter = 0;
    RV_ASSERT(shader == material->pShader, "shader given and the materials shader are not the same shader");
    shader->bind();
    material->bind();
}

void RenderManager::onEvent(Event* event)
{
    RV_ASSERT(event->m_Type == EventTypeWindowResize, "event not recognized");
    if(event->m_Type == EventTypeWindowResize)
    {
        EventWindowResize* resizeEvent = (EventWindowResize*)event;
        get()->deffered.resize(resizeEvent->m_Width, resizeEvent->m_Height);
    }

}

void RenderManager::iShutdown() 
{
}


/*
void RenderManager::shadowMapRenderPass() //TODO: ukloniti ovaj ogroman komentar
{

    for(auto itLight = Scene::getEntityList()->begin(); itLight != Scene::getEntityList()->end(); itLight++)
    {
        if(!itLight->valid)
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

        if(pLight->lightType == LightType::LightTypeDirectional)
        {
            directionalShadowMapShader.bind();
        }
        else {
            RV_ASSERT(pLight->lightType == LightType::LightTypePoint, "unrecognized light type");
            omnidirectionalShadowMapShader.bind();
        }

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
                //cout << "ShadowMapping light: " << itLight->entityName << " opaque: " << itOpaque->entityName << endl;
                Model* pModel = &itOpaque->get<ModelComponent>()->model;
                TransformComponent* pTransformComponent = itOpaque->get<TransformComponent>();

                if(pLight->lightType == LightType::LightTypeDirectional)
                {
                    directionalShadowMapShader.uploadUniformMat4("u_ModelMatrix", pTransformComponent->getTransform());
                    directionalShadowMapShader.uploadUniformMat4("u_ShadowMapViewMatrix", pShadowMap->camera.viewMatrix);
                    directionalShadowMapShader.uploadUniformMat4("u_ShadowMapProjectionMatrix", pShadowMap->camera.projectionMatrix);

                    for(unsigned int i=0; i < pModel->pMeshes.size(); i++)
                    {
                        pModel->pMeshes[i]->vao.bind();
                        RenderCommand::drawElements(*pModel->pMeshes[i]);
                    }
                }
                else {
                    omnidirectionalShadowMapShader.uploadUniformMat4("u_ModelMatrix", pTransformComponent->getTransform());
                    omnidirectionalShadowMapShader.uploadUniform3f("u_LightPosition", itLight->get<TransformComponent>()->position);
                    omnidirectionalShadowMapShader.uploadUniform1f("u_FarPlane", pLight->getShadowMap()->renderDistance);
                    
                    Camera tempCamera;
                    tempCamera.renderDistance = pLight->getShadowMap()->renderDistance;
                    tempCamera.nearRenderDistance = pLight->getShadowMap()->nearRenderDistance;
                    tempCamera.setPerspectiveProjection(degreesToRadians(90), 1.f);
                    omnidirectionalShadowMapShader.uploadUniformMat4("u_ShadowProjectionMatrix", tempCamera.projectionMatrix);

                    Vec3f lightPos = itLight->get<TransformComponent>()->position;
                    std::vector<Vec3f> shadowDirections;

                    if(1) // moj
                    {
                        shadowDirections.push_back(Vec3f(0, -1, 0));
                        shadowDirections.push_back(Vec3f(0, 1, 0));
                        shadowDirections.push_back(Vec3f(0, 0, -1));
                        shadowDirections.push_back(Vec3f(0, 0, 1));
                        shadowDirections.push_back(Vec3f(1, 0, 0));
                        shadowDirections.push_back(Vec3f(-1, 0, 0));
                    }
                    else {
                        shadowDirections.push_back(Vec3f(1, 0, 0));
                        shadowDirections.push_back(Vec3f(-1, 0, 0));
                        shadowDirections.push_back(Vec3f(0, 1.0, 0));
                        shadowDirections.push_back(Vec3f(0, -1.0, 0));
                        shadowDirections.push_back(Vec3f(0, 0, 1.0));
                        shadowDirections.push_back(Vec3f(0, 0, -1.0));
                    }

                    for(int face=0; face<6; face++)
                    {
                        std::string baseString = "u_ShadowViewMatrix";
                        tempCamera.setViewMatrix(lightPos, lookAtGetRotation(lightPos, lightPos + shadowDirections[face]));
                        for(unsigned int i=0; i < pModel->pMeshes.size(); i++)
                        {
                            pModel->pMeshes[i]->vao.bind();
                            RenderCommand::drawElements(*pModel->pMeshes[i]);
                        }
                    }
                }
            }
        }
    }
}
*/
