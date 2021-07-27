#include<reviv.h>

Entity *player, *camera, *stanic, *light, *platform, *sphere, *cube;
Entity* map;

ModelLoader modelLoaderBackpack, modelLoaderMap;
Material materialStanic;

bool useMap = false;

class Sandbox : public Application
{
public:

    void initBeforeEngine() override
    {
        camera = Scene::setCameraEntity(Scene::createEntity("Camera"));
        camera->add<CameraComponent>(0.01f, 5000.f, degreesToRadians(100.f));

        player = Scene::setPlayerEntity(Scene::createEntity("Player"));
        player->get<TransformComponent>()->position = {0, 0, 3};

        if(useMap)
        {
            map = Scene::createEntity("Sponza");
            map->get<TransformComponent>()->scale = {0.005f, 0.005f, 0.005f};
        }

        stanic = Scene::createEntity("Stanic");
        stanic->get<TransformComponent>()->position = {1, 0, 2};

        light = Scene::createEntity("Light");
        light->get<TransformComponent>()->scale = {0.2f, 0.2f, 0.2f};

        platform = Scene::createEntity("Platform");
        platform->get<TransformComponent>()->scale = {7, 14, 0.4};

        sphere = Scene::createEntity("Sphere");
        sphere->get<TransformComponent>()->position = {1, 2, 1};
        sphere->get<TransformComponent>()->scale = {0.3f, 0.3f, 0.3f};

        cube = Scene::createEntity("Cube");
        //cube->get<TransformComponent>()->position = {3, 2, 1.5};
        cube->get<TransformComponent>()->position = {10, 0, 3};
        cube->get<TransformComponent>()->scale = {0.3, 0.3, 0.3};

        materialStanic.setShader(&AssetManager::get()->shaderPhong);
    }

    void initAfterEngine() override
    {
        modelLoaderBackpack.load("assets/models/backpack/backpack.obj");
        if(useMap) modelLoaderMap.load("assets/sponza/scene.gltf");

        auto* pPointLight = light->add<PointLightComponent>();
        pPointLight->light.ambient = Vec3f(1, 1, 1); // 
        pPointLight->light.diffuse = Vec3f(0.2, 0.2, 0.2);
        //pPointLight->light.specular = Vec3f(0.5, 0.5, 0.5); // 
        pPointLight->light.specular = Vec3f(0.05, 0.05, 0.05); // 

        light->add<ModelComponent>          (&AssetManager::get()->modelLoaderSphere);
        stanic->add<ModelComponent>         (&modelLoaderBackpack,                      &AssetManager::get()->materialEmerald);
        platform->add<ModelComponent>       (&AssetManager::get()->modelLoaderCube,     &AssetManager::get()->materialGold);
        sphere->add<ModelComponent>         (&AssetManager::get()->modelLoaderSphere,   &AssetManager::get()->materialRuby);
        cube->add<ModelComponent>           (&AssetManager::get()->modelLoaderCube,     &AssetManager::get()->materialObsidian);
        if(useMap) map->add<ModelComponent> (&modelLoaderMap, &AssetManager::get()->materialTurquoise);

        light->get<ModelComponent>()->model.addMaterialFromShader(AssetManager::get()->shaderMonochroma);
        light->get<ModelComponent>()->model.pMaterials[0]->set("u_Color", Vec3f(1, 1, 1));

        //textureCube.init("assets/textures/container.png");
    }

    void onUpdate() override
    {
        light->get<TransformComponent>()->position = Vec3f(sin(Time::getTime() / 2) * 2, cos(Time::getTime() / 2) * 2, 2 + sin(Time::getTime() / 5));

        cube->get<TransformComponent>()->rotation.yaw = Time::getTime() * 7;
        cube->get<TransformComponent>()->rotation.pitch = Time::getTime() * 11;
        cube->get<TransformComponent>()->rotation.roll = Time::getTime() * 3;
        //cube->get<TransformComponent>()->rotation.roll = Time::getTime();
        //cube->get<TransformComponent>()->rotation.pitch = Time::getTime() * 3 + 0.4;
        
        //light->get<TransformComponent>()->position = Vec3f(sin(Time::getTime() / 100) * 2, cos(Time::getTime() / 100) * 2, 2 + sin(Time::getTime() / 5));

        if(Time::isOneSecond()){
            cout << "FPS: " << 1 / Time::getDelta() << endl;
            cout << "Position: ";
            log(Scene::getPlayerEntity()->get<TransformComponent>()->position);
            cout << "Rotation: ";
            log(Scene::getPlayerEntity()->get<TransformComponent>()->rotation);

            cout << "BEGIN----------- " << endl;
            cout << "cubeTransform" << endl;
            log(cube->get<TransformComponent>()->getTransform());

            cout << "afterTransform" << endl;
            auto afterTransform = multiply(cube->get<TransformComponent>()->getTransform(), Vec4f(0, 0, 0, 1));
            log(afterTransform);

            cout << "afterView" << endl;
            auto afterView = multiply(Scene::getCameraEntity()->get<CameraComponent>()->camera.viewMatrix, afterTransform);
            log(afterView);

            cout << "afterProjection" << endl;
            auto afterProjection = multiply(Scene::getCameraEntity()->get<CameraComponent>()->camera.projectionMatrix, afterView);
            log(afterProjection);

            cout << "afterNdc" << endl;
            auto afterNdc = afterProjection / afterProjection.a[3];
            log(afterNdc);
            cout << "------------------" << endl;

            cout << "cubePosition:" << endl;
            log(cube->get<TransformComponent>()->position);
            cout << endl;
        }

       

        

        //stanicTexture.setUp("../resources/textures/stene.png");
        //stanicTexture.bind(0);
        //beloTexture.setUp("../resources/textures/belo.png");
    }

};

Application* createApplication()
{
    return new Sandbox();
}