#include<reviv.h>

Entity *player, *camera, *stanic, *light, *platform;

Shader shaderPhong, shaderMonochroma;
Material materialPhong, materialLight;
ModelLoader modelLoaderBackpack, modelLoaderCube;

Material materialNov;
Shader shaderNov;
Entity* nov;
ModelLoader modelLoaderNov;
Model modelNov;

Model* stanicModel;
Material* stanicMaterial;

class Sandbox : public Application
{
public:

    void initBeforeEngine() override
    {
        camera = Scene::setCameraEntity(Scene::createEntity("Camera"));
        player = Scene::setPlayerEntity(Scene::createEntity("Player"));

        camera->add<CameraComponent>(0.001f, 1000.f, degreesToRadians(100.f));

        stanic = Scene::createEntity("Stanic");
        stanic->get<TransformComponent>()->position = {1, 0, 2};

        light = Scene::createEntity("Light");
        light->get<TransformComponent>()->scale = {0.1f, 0.1f, 0.1f};

        platform = Scene::createEntity("Platform");
        platform->get<TransformComponent>()->scale = {7, 14, 0.4};
        platform->get<TransformComponent>()->position = {0, 0, -0.4};
    }

    void initAfterEngine() override
    {
        shaderMonochroma.init("assets/shaders/monochroma.vs", "assets/shaders/monochroma.fs");
        shaderPhong.init("assets/shaders/phong.vs", "assets/shaders/phong.fs");
        shaderNov.init("assets/shaders/novvs.txt", "assets/shaders/novfs.txt");

        materialPhong.setShader(&shaderPhong);
        materialNov.setShader(&shaderNov);
        materialLight.setShader(&shaderMonochroma);

        modelLoaderBackpack.load("assets/models/backpack/backpack.obj");
        modelLoaderCube.load("assets/models/cube.obj");
        modelLoaderNov.load("assets/models/nov.obj");

        modelNov = Model(&modelLoaderNov, &materialNov);

        stanic->add<ModelComponent>(&modelLoaderBackpack, &materialPhong);
        light->add<ModelComponent>(&modelLoaderCube, &materialLight);
        platform->add<ModelComponent>(&modelLoaderCube);

        Material* platformMaterial = platform->get<ModelComponent>()->model.addMaterial(shaderPhong);
        platformMaterial->set("u_Color", Vec3f(0.8f, 0.8f, 0.8f));

        stanicModel = &stanic->get<ModelComponent>()->model;
        stanicMaterial = stanicModel->pMaterials[0];
        stanicMaterial->set("u_Color", Vec3f(0.8, 0.2, 0.9));

        materialLight.set("u_Color", Vec3f(1, 1, 1));
    }

    void onUpdate() override
    {
        //light->get<TransformComponent>()->position = Vec3f(sin(Time::getTime() * 5) * 5, cos(Time::getTime() * 5) * 5, 5 + sin(Time::getTime() / 2));
        light->get<TransformComponent>()->position = Vec3f(sin(Time::getTime() * 2) * 2, cos(Time::getTime() * 2) * 2, 2 + sin(Time::getTime() / 2));

        if(std::isnan(player->get<TransformComponent>()->position.x))
        {
            RV_ASSERT(false, "");
        }

        if(Time::isOneSecond()){
            cout << "FPS: " << 1 / Time::getDelta() << endl;
            cout << "Position: ";
            log(Scene::getPlayerEntity()->get<TransformComponent>()->position);
            cout << "Rotation: ";
            log(Scene::getPlayerEntity()->get<TransformComponent>()->rotation);
        }

        //stanicMaterial->set("u_LightPosition", light->get<TransformComponent>()->position);
        platform->get<ModelComponent>()->model.pMaterials[0]->set("u_LightPosition", light->get<TransformComponent>()->position);

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