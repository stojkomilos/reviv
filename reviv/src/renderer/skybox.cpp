#include"skybox.h"
#include"render_manager.h"

void Skybox::init(const std::vector<std::string>& filePaths)
{
    cubeModel.load("assets/models/cube.fbx");
    shader.init("assets/shaders/skybox.vs", "assets/shaders/skybox.fs");
    material.setShader(&shader);
    textureCubeMap.load(filePaths);

    material.setTexture("u_CubeMap", textureCubeMap);
}

void Skybox::onUpdate(Framebuffer* pFramebuffer)
{
    pFramebuffer->bind();

    glDisable(GL_CULL_FACE);
    glDepthFunc(GL_LEQUAL);
    
    RenderManager::get()->bindMaterial(material.pShader, &material);

    auto* pCamera = &Scene::getCameraEntity()->get<CameraComponent>()->camera;
    Mat<4,4> skyboxViewMatrix;
    skyboxViewMatrix = pCamera->viewMatrix;
    *skyboxViewMatrix.getPtr(0, 3) = *skyboxViewMatrix.getPtr(1, 3) = *skyboxViewMatrix.getPtr(2, 3) = 0;
    material.pShader->uploadUniformMat4("u_SkyboxViewMatrix", skyboxViewMatrix);
    material.pShader->uploadUniformMat4("u_ProjectionMatrix", pCamera->projectionMatrix);

    cubeModel.meshes[0].vao.bind();
    RenderCommand::get()->drawElements(cubeModel.meshes[0]);

    glDepthFunc(GL_LESS);
}
