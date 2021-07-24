#include<reviv.h>

Entity *player, *camera, *stanic;
Shader shaderTexture, shaderMonochroma;
ModelLoader modelLoaderBackpack;
Material materialBasic;

Material materialNov;
Shader shaderNov;
Entity* nov;
ModelLoader modelLoaderNov;
Model modelNov;


class Sandbox : public Application
{
public:

    void initBeforeEngine() override
    {
        camera = Scene::setCameraEntity(Scene::createEntity("Camera"));
        player = Scene::setPlayerEntity(Scene::createEntity("Player"));

        camera->add<PositionComponent>();
        camera->add<RotationComponent>();
        camera->add<CameraComponent>(0.001f, 1000.f, degreesToRadians(100.f));

        player->add<PositionComponent>(0, 0, 0);
        player->add<RotationComponent>(Vec3f(0, 0, 0));

        stanic = Scene::createEntity("Stanic");
        stanic->add<PositionComponent>(1, 0, 0);
        stanic->add<TransformComponent>();
        stanic->add<RotationComponent>(Vec3f(0, 0, 0));

    }

    void initAfterEngine() override
    {
        shaderMonochroma.init("assets/shaders/monochroma.vs", "assets/shaders/monochroma.fs");
        shaderNov.init("assets/shaders/novvs.txt", "assets/shaders/novfs.txt");

        materialBasic.setShader(&shaderMonochroma);
        materialNov.setShader(&shaderNov);
      
        modelLoaderBackpack.load("assets/models/backpack/backpack.obj");
        stanic->add<ModelComponent>(&modelLoaderBackpack, &materialBasic);

        modelLoaderNov.load("assets/models/nov.obj");
        modelNov = Model(&modelLoaderNov, &materialNov);
    }

    void onUpdate() override
    {
        //stanic->get<PositionComponent>()->position = Vec3f(sin(Time::getTime()), 0, cos(Time::getTime()));

        if(std::isnan(player->get<PositionComponent>()->position.x))
        {
            RV_ASSERT(false, "");
        }

        if(Time::isOneSecond()){
            cout << "FPS: " << 1 / Time::getDelta() << endl;
            cout << "Position: ";
            log(Scene::getPlayerEntity()->get<PositionComponent>()->position);
            cout << "Rotation: ";
            log(Scene::getPlayerEntity()->get<RotationComponent>()->rotation);

        }

        auto* stanicModel = &stanic->get<ModelComponent>()->model;

        materialBasic.pShader->bind();
        materialBasic.set("u_Color", Vec4f(0, 0, 1, 1));
        materialBasic.set("u_Projection", Scene::getCameraEntity()->get<CameraComponent>()->camera.projectionMatrix);
        materialBasic.set("u_View", Scene::getCameraEntity()->get<CameraComponent>()->camera.viewMatrix);

        materialBasic.set("u_Model", stanic->get<TransformComponent>()->transform);

        //materialNov.bind();
        //modelNov.pMeshes[0]->vao.bind();
        //glDrawElements(GL_TRIANGLES, modelNov.pMeshes[0]->m_Indices.size(), GL_UNSIGNED_INT, 0);

        //stanicTexture.setUp("../resources/textures/stene.png");
        //stanicTexture.bind(0);
        //beloTexture.setUp("../resources/textures/belo.png");
    }

};

Application* createApplication()
{
    return new Sandbox();
}