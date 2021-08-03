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
    gPosition.init();
    gPosition.bind(0);
    ////////////////////////////////////////////////////// BAD WIDTH PROBABLY
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screenWidth, screenHeight, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition.id, 0);

    gNormal.init();
    gNormal.bind(1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screenWidth, screenHeight, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal.id, 0);

    gAlbedoSpecular.init();
    gAlbedoSpecular.bind(2);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screenWidth, screenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpecular.id, 0);

    unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    glDrawBuffers(3, attachments);

    gBuffer.unbind();

    defferedShader.init("assets/shaders/deffered.vs", "assets/shaders/deffered.fs");

}

void RenderManager::iOnUpdate()
{
    beginScene(); // Update camera stuff
    
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
                    if(pModel->pMaterials[0]->pShader == &AssetManager::get()->shaderDeffered)
                    {
                        pModel->pMaterials[i]->bind();
                        environment.set("u_ModelMatrix", pTransformComponent->getTransform());
                        environment.bind(*pModel->pMaterials[i]->pShader);

                        pModel->pMeshes[i]->vao.bind();

                        RenderCommand::drawElements(*pModel->pMeshes[i]);
                    }
                }
            }
        }
    }

    gBuffer.unbind();
    AssetManager::get()->shaderDefferedBlinnPhong.bind();

    AssetManager::get()->shaderDefferedBlinnPhong.uploadUniform1i("u_gPosition", 0);
    AssetManager::get()->shaderDefferedBlinnPhong.uploadUniform1i("u_gNormal", 1);
    AssetManager::get()->shaderDefferedBlinnPhong.uploadUniform1i("u_gAlbedoSpecular", 2);

    environment.bind(AssetManager::get()->shaderDefferedBlinnPhong);

    RenderCommand::clear();
    gPosition.bind(0);
    gNormal.bind(1);
    gAlbedoSpecular.bind(2);
    AssetManager::get()->modelLoaderQuad2D.meshes[0].vao.bind();
    RenderCommand::drawElements(AssetManager::get()->modelLoaderQuad2D.meshes[0]);
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

    environment.set("u_ViewMatrix", camera->viewMatrix);
    environment.set("u_ProjectionMatrix", camera->projectionMatrix);

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
                    continue;
                    RV_ASSERT(false, "");
                }

                std::string uniformNameLight = "u_PointLights[";
                uniformNameLight += (char)(nrLight + '0');
                uniformNameLight += "]";
                cout << "lightUniform: " << uniformNameLight << endl;

                set(uniformNameLight + ".position", itEntity->get<TransformComponent>()->position);

                set(uniformNameLight + ".ambient", pLight->ambient);
                set(uniformNameLight + ".diffuse", pLight->diffuse);
                set(uniformNameLight + ".specular", pLight->specular);

                set(uniformNameLight + ".constant", pLight->constant);
                set(uniformNameLight + ".linear", pLight->linear);
                set(uniformNameLight + ".quadratic", pLight->quadratic);

                set("u_ViewPosition", Scene::getPlayerEntity()->get<TransformComponent>()->position);

                nrLight++;
            }
        }
    }
}

/*
void RenderManager::submit(const Model& model, const Mat4& transform)
{
    RV_ASSERT(model.pMeshes.size() > 0 && model.pMaterials.size() > 0, "");
    RV_ASSERT(model.pMaterials.size() == model.pMeshes.size(), "");
    //RV_ASSERT(model.pMaterials.size() == model.pMeshes.size() || model.pMaterials.size() == 1, ""); // every mesh should have an equivalent material, OR there is just 1 material for every mesh

    if(model.pMaterials[0]->pShader == &AssetManager::get()->shaderDefferedBlinnPhong)
    for(unsigned int i=0; i<model.pMeshes.size(); i++)
    {
        model.pMaterials[i]->bind();
        bindEnvironment(*model.pMaterials[i]->pShader, transform);
        model.pMeshes[i]->vao.bind();

        RenderCommand::drawElements(*model.pMeshes[i]);
    }
}
*/

void Environment::bind(const Shader& shader) const
{
    shader.bind();
    shaderUniformMap.bind(shader);
}

void RenderManager::iShutdown() 
{
}