#include"render_manager.h"

extern Entity* directionalLight;

Camera lightCamera(0.1f, 20.f, 90);

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

    shadowMap.init(1000, 1000);
    shadowMapShader.init("assets/shaders/shadow_map.vs", "assets/shaders/shadow_map.fs");

    //depthTestShader.init("assets/shaders/screen_framebuffer.vs", "assets/shaders/depth_test.fs");
}

void RenderManager::iOnUpdate()
{
    /*
    if(Time::getLoopCounter() == 3)
    {
        screenFramebuffer.init();
        screenFramebuffer.attachTexture();
        screenFramebuffer.attachRenderbuffer();
        screenShader.init("assets/shaders/screen_framebuffer.vs", "assets/shaders/screen_framebuffer.fs");
    }
    if(Time::getLoopCounter() >= 3)
    {
        screenFramebuffer.bind();
        glEnable(GL_DEPTH_TEST); // TODO: staviti gore
    }
    */
    // ----

    shadowMap.framebuffer.bind();
    glClear(GL_DEPTH_BUFFER_BIT);
    lightCamera.setViewMatrix(directionalLight->get<TransformComponent>()->position, directionalLight->get<TransformComponent>()->rotation);
    //lightCamera.setPerspectiveProjection();
    lightCamera.setOrthographicProjection(14);
    shadowMapShader.bind();
    shadowMapShader.uploadUniformMat4("u_ViewMatrix", lightCamera.viewMatrix);
    shadowMapShader.uploadUniformMat4("u_ProjectionMatrix", lightCamera.projectionMatrix);
    glViewport(0, 0, 1000, 1000);

    for(auto itEntity = Scene::getEntityList()->begin(); itEntity != Scene::getEntityList()->end(); itEntity++)
    {
        if(itEntity->valid)
        {
            if(itEntity->has<ModelComponent>() && itEntity->has<TransformComponent>())
            {
                cout << "Shadow for entity: " << itEntity->entityName << endl;
                Model* pModel = &itEntity->get<ModelComponent>()->model;
                TransformComponent* pTransform = itEntity->get<TransformComponent>();

                for(unsigned int i=0; i < pModel->pMeshes.size(); i++)
                {
                    shadowMapShader.uploadUniformMat4("u_ModelMatrix", pTransform->getTransform());
                    pModel->pMeshes[i]->vao.bind();
                    RenderCommand::drawElements(*pModel->pMeshes[i]);
                }
            }
        }
    }
    endScene();

    /*
    shadowMap.framebuffer.unbind();
    int tempWidth = Application::get()->getWindow()->m_Data.width;
    int tempHeight = Application::get()->getWindow()->m_Data.height;
    glViewport(0, 0, tempWidth, tempHeight);
    RenderCommand::setClearColor(Vec4f(100.f, 10.f, 80.f, 256.f) / 256.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //config;
    shadowMap.depthMap.bind(0);
    shadowMap.framebuffer.unbind();
    depthTestShader.bind();

    auto* pMesh = &AssetManager::get()->modelLoaderQuad2D.meshes[0];
    pMesh->vao.bind();
    RenderCommand::drawElements(*pMesh);
    */

    //// RENDERING REGULAR SCENE:

    //skybox.onUpdate();

    /// ---------------

    
    shadowMap.framebuffer.unbind();
    int tempWidth = Application::get()->getWindow()->m_Data.width;
    int tempHeight = Application::get()->getWindow()->m_Data.height;
    glViewport(0, 0, tempWidth, tempHeight);
    //config;
    shadowMap.depthMap.bind(0);


    //// ------

    RenderCommand::setClearColor(Vec4f(100.f, 10.f, 80.f, 256.f) / 256.f);
    RenderCommand::clear();

    beginScene(); // Update camera stuff

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
    
    
    //if(Time::getLoopCounter() >= 3)
    //{
    //    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //    RenderCommand::setClearColor(Vec4f(1, 1, 1, 1));
    //    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //    screenShader.bind();
    //    //glDisable(GL_DEPTH_TEST);
    //    screenFramebuffer.pTexture->bind(0);

    //    AssetManager::get()->modelLoaderQuad2D.meshes[0].vao.bind();
    //    //glDrawElements(GL_TRIANGLES, AssetManager::get()->modelLoaderQuad2D.meshes[0].m_Indices.size(), GL_UNSIGNED_INT, 0);

    //    RenderCommand::drawElements(AssetManager::get()->modelLoaderQuad2D.meshes[0]);
    //}
    
   
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
        RV_ASSERT(&shader == &AssetManager::get()->shaderMonochroma || &shader == &AssetManager::get()->shaderTexture, "");
        return;
    }

    shader.uploadUniformMat4("u_LightViewMatrix", lightCamera.viewMatrix);
    shader.uploadUniformMat4("u_LightProjectionMatrix", lightCamera.projectionMatrix);
    //shader.uploadUniform1i("u_ShadowMap", 0);

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

                nrLight++;
            }
        }
    }
}

void RenderManager::iShutdown() 
{
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

}

void RenderManager::endScene()
{

}