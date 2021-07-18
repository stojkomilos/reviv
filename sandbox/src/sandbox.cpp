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

        player->add<PositionComponent>(0, 0, 0);
        player->add<RotationComponent>(Vec3f(0, 0, 0));

        stanic = Scene::createEntity("Stanic");
        stanic->add<PositionComponent>(3, 0, 0);
        stanic->add<TransformComponent>();
        stanic->add<RotationComponent>();

    }

    void initAfterEngine() override
    {
        shaderTexture.init      ("assets/shaders/texture.vs", "assets/shaders/texture.fs");
        shaderMonochroma.init("assets/shaders/monochroma.vs", "assets/shaders/monochroma.fs");

        stanic->add<VaoComponent>("assets/models/weapon.obj");
        stanic->add<MaterialComponent>(&shaderMonochroma);
    }

    void onUpdate() override
    {
        //cout << "sandbox onUpdate()" << endl;

        //stanic->get<PositionComponent>()->position = Vec3f(sin(Time::getTimeInSeconds()), 0, cos(Time::getTimeInSeconds()));

        if(Time::isOneSecond()){
            cout << "FPS: " << 1 / Time::getDelta() << endl;
            cout << "Position: ";
            log(Scene::getPlayerEntity()->get<PositionComponent>()->position);
        }


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