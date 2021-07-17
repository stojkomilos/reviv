#include<reviv.h>

#include<iostream>

Entity *player, *camera, *stanic;

Shader shaderTexture, shaderMonochroma;

class Sandbox : public Application
{
public:

    void initBeforeEngine() override
    {
        cout << "sandbox init()" << endl;;
        camera = Scene::setCameraEntity(Scene::createEntity("Camera"));
        player = Scene::setPlayerEntity(Scene::createEntity("Player"));

        camera->add<PositionComponent>();
        camera->add<RotationComponent>();
        camera->add<CameraComponent>(0.1f, 1000, 60.0f / 180.0f * 2.0f * 3.14f);

        player->add<PositionComponent>(1, 1, 1);
        player->add<RotationComponent>(Vec3f(0, 0, 0));

        stanic = Scene::createEntity("Stanic");
        stanic->add<PositionComponent>();
        stanic->add<TransformComponent>();

    }

    void initAfterEngine() override
    {
        shaderTexture.init      ("assets/shaders/texture.vs", "assets/shaders/texture.fs");
        shaderMonochroma.init("assets/shaders/monochroma.vs", "assets/shaders/monochroma.fs");

        stanic->add<VaoComponent>("assets/models/sphere.obj");
        stanic->add<MaterialComponent>(&shaderMonochroma);
    }

    void onUpdate() override
    {
        //cout << "sandbox onUpdate()" << endl;

        stanic->get<PositionComponent>()->position = add(player->get<PositionComponent>()->position, Vec3f(5 * sin(Time::getTimeInSeconds() * 5), 0, 5 * cos(Time::getTimeInSeconds() * 5)));

        auto* stanicMaterial = &stanic->get<MaterialComponent>()->material;
        stanicMaterial->pShader->bind();
        stanicMaterial->set("u_Color", Vec4f(0, 0, 1, 1));
        stanicMaterial->set("u_Projection", Scene::getCameraEntity()->get<CameraComponent>()->camera.projectionMatrix);
        stanicMaterial->set("u_View", Scene::getCameraEntity()->get<CameraComponent>()->camera.viewMatrix);
        stanicMaterial->set("u_Model", stanic->get<TransformComponent>()->transform);

    }

};

Application* createApplication()
{
    return new Sandbox();
}