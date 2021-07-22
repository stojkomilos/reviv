#include<reviv.h>

Entity *player, *camera, *stanic;
Shader shaderTexture, shaderMonochroma;
ModelLoader modelLoaderBackpack;
Material materialBasic;

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
        stanic->add<PositionComponent>(1, 0, 0);
        stanic->add<TransformComponent>();
        stanic->add<RotationComponent>(Vec3f(0, 0, 0));

    }

    void initAfterEngine() override
    {
        shaderTexture.init      ("assets/shaders/texture.vs", "assets/shaders/texture.fs");
        shaderMonochroma.init("assets/shaders/monochroma.vs", "assets/shaders/monochroma.fs");

        //stanic->add<VaoComponent>("assets/models/weapon.obj");
        materialBasic.setShader(&shaderMonochroma);

        //stanic->add<MaterialComponent>(&shaderMonochroma);
        //stanic->add<ModelComponent>("assets/models/weapon.obj");

        //modelLoaderBackpack.load("assets/models/backpack/backpack.obj");
        //modelLoaderBackpack.load("assets/models/Human/human.obj");
        modelLoaderBackpack.load("assets/models/weapon.obj");
        //modelLoaderBackpack.load("assets/models/weapon.obj");

        stanic->add<ModelComponent>(&modelLoaderBackpack, &materialBasic);
    }

    void onUpdate() override
    {
        //stanic->get<PositionComponent>()->position = Vec3f(sin(Time::getTime()), 0, cos(Time::getTime()));

        if(Time::isOneSecond()){
            cout << "FPS: " << 1 / Time::getDelta() << endl;
            cout << "Position: ";
            //log(Scene::getPlayerEntity()->get<PositionComponent>()->position);
        }

        auto* stanicModel = &stanic->get<ModelComponent>()->model;

        //auto* stanicMaterial = &stanic->get<MaterialComponent>()->material;
        materialBasic.pShader->bind();
        materialBasic.set("u_Color", Vec4f(0, 0, 1, 1));
        materialBasic.set("u_Projection", Scene::getCameraEntity()->get<CameraComponent>()->camera.projectionMatrix);
        materialBasic.set("u_View", Scene::getCameraEntity()->get<CameraComponent>()->camera.viewMatrix);

        materialBasic.set("u_Model", stanic->get<TransformComponent>()->transform);
        //log(*stanicModel);
        //log(stanic->get<ModelComponent>()->model);

        //materialBasic.set("u_Model", translate(identity, stanic->get<PositionComponent>()->position));

        //stanicTexture.setUp("../resources/textures/stene.png");
        //stanicTexture.bind(0);
        //beloTexture.setUp("../resources/textures/belo.png");
    }

};

Application* createApplication()
{
    return new Sandbox();
}