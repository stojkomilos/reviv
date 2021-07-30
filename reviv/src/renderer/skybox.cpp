#include"skybox.h"

void Skybox::init(const std::vector<std::string>& filePaths)
{
    cubeModel.load("assets/models/cube.fbx");
    shader.init("assets/shaders/cubemap.vs", "assets/shaders/cubemap.fs");
    textureCubeMap.load(filePaths);
}

void Skybox::onUpdate()
{
    glDisable(GL_CULL_FACE);
    glDepthFunc(GL_LEQUAL);
    shader.bind();
    auto* pCamera = &Scene::getCameraEntity()->get<CameraComponent>()->camera;
    Mat4 skyboxViewMatrix;
    skyboxViewMatrix = pCamera->viewMatrix;
    skyboxViewMatrix.a[0][3] = skyboxViewMatrix.a[1][3] = skyboxViewMatrix.a[2][3] = 0;
    shader.uploadUniformMat4("u_ViewMatrix", skyboxViewMatrix);
    shader.uploadUniformMat4("u_ProjectionMatrix", pCamera->projectionMatrix);
    shader.uploadUniform1i("u_CubeMap", 0); // TODO: ne moze na slot 0 jer druge teksture to mozda budu koristile
    textureCubeMap.bind(0);

    cubeModel.meshes[0].vao.bind();
    RenderCommand::drawElements(cubeModel.meshes[0]);

    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
}
