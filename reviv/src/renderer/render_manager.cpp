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
    //skybox.init(skyboxFaces);

    gBuffer.init();
    gBuffer.bind();

    int screenWidth = Application::get()->getWindow()->m_Data.width;
    int screenHeight = Application::get()->getWindow()->m_Data.height;
    screenWidth = 956;
    screenHeight = 1050;

    gDepth.init();
    gDepth.bind(0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, screenWidth, screenHeight, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL); // using stencil even though i don't use it because of driver comptability sutff
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, gDepth.id, 0);

    gPosition.init();
    gPosition.bind(0);
    ////////////////////////////////////////////////////// BAD WIDTH PROBABLY
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screenWidth, screenHeight, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition.id, 0);

    gNormal.init();
    gNormal.bind(0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screenWidth, screenHeight, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal.id, 0);

    gAlbedoSpecular.init();
    gAlbedoSpecular.bind(0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screenWidth, screenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpecular.id, 0);

    unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    glDrawBuffers(3, attachments);

    gBuffer.unbind();

    shaderDeffered.init("assets/shaders/deffered.vs", "assets/shaders/deffered.fs");


    shaderDefferedBlinnPhong.init("assets/shaders/deffered_blinn_phong.vs", "assets/shaders/deffered_blinn_phong.fs");
    materialDefferedBlinnPhong.setShader(&shaderDefferedBlinnPhong);
    materialDefferedBlinnPhong.addTexture("u_gPosition", gPosition);
    materialDefferedBlinnPhong.addTexture("u_gNormal", gNormal);
    materialDefferedBlinnPhong.addTexture("u_gAlbedoSpecular", gAlbedoSpecular);
}

void RenderManager::iOnUpdate()
{
    beginScene(); // Update camera stuff, update environment uniforms(doesn't upload them)
    
    gBuffer.bind();

    RenderCommand::setClearColor(Vec4f(100.f, 10.f, 80.f, 256.f) / 256.f);
    RenderCommand::clear();

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
                    if(pModel->pMaterials[0]->pShader == &shaderDeffered)
                    {
                        pModel->pMaterials[i]->bind();
                        pModel->pMaterials[i]->pShader->uploadUniformMat4("u_ModelMatrix", pTransformComponent->getTransform());
                        environment.bind(*pModel->pMaterials[i]->pShader);

                        pModel->pMeshes[i]->vao.bind();

                        RenderCommand::drawElements(*pModel->pMeshes[i]);
                    }
                }
            }
        }
    }

    gBuffer.unbind();
    materialDefferedBlinnPhong.bind();
    //AssetManager::get()->shaderDefferedBlinnPhong.bind();
    environment.bind(shaderDefferedBlinnPhong);

    RenderCommand::clear();
    AssetManager::get()->modelLoaderQuad2D.meshes[0].vao.bind();
    RenderCommand::drawElements(AssetManager::get()->modelLoaderQuad2D.meshes[0]);

    glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer.id);
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
                        pModel->pMaterials[i]->bind();
                        pModel->pMaterials[i]->pShader->uploadUniformMat4("u_ModelMatrix", pTransformComponent->getTransform());
                        environment.bind(*pModel->pMaterials[i]->pShader);

                        pModel->pMeshes[i]->vao.bind();

                        RenderCommand::drawElements(*pModel->pMeshes[i]);
                    }
                }
            }
        }
    }
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

    environment.set("ue_ViewMatrix", camera->viewMatrix);
    environment.set("ue_ProjectionMatrix", camera->projectionMatrix);
    
    environment.set("ue_ViewPosition", Scene::getCameraEntity()->get<TransformComponent>()->position);

    environment.setLights();
}

void Environment::setLights()
{
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
                    RV_ASSERT(false, "");
                    continue;
                }

                std::string uniformNameLight = "ue_PointLights[";
                uniformNameLight += (char)(nrLight + '0');
                uniformNameLight += "]";

                set(uniformNameLight + ".position", itEntity->get<TransformComponent>()->position);

                set(uniformNameLight + ".ambient", pLight->ambient);
                set(uniformNameLight + ".diffuse", pLight->diffuse);
                set(uniformNameLight + ".specular", pLight->specular);

                set(uniformNameLight + ".constant", pLight->constant);
                set(uniformNameLight + ".linear", pLight->linear);
                set(uniformNameLight + ".quadratic", pLight->quadratic);

                nrLight++;
            }
        }
    }
}

void Environment::bind(const Shader& shader) const
{
    shader.bind();
    for(auto it = shader.uniformNames.begin(); it != shader.uniformNames.end(); it++)
    {
        if((*it)[0] == 'u' and (*it)[1] == 'e' and (*it)[2] == '_')
            shaderUniformMap.uploadUniform(shader, *it);
    }
}

void RenderManager::iShutdown() 
{
}