#include<reviv.h>

Entity *player, *camera, *stanic, *light, *platform, *sphere, *cube;
Entity* map;

ModelLoader modelLoaderBackpack, modelLoaderMap;
Material materialStanic;

//Texture textureCube;

bool useMap = false;

class Sandbox : public Application
{
public:

    void initBeforeEngine() override
    {
        camera = Scene::setCameraEntity(Scene::createEntity("Camera"));
        camera->add<CameraComponent>(0.001f, 1000.f, degreesToRadians(100.f));

        player = Scene::setPlayerEntity(Scene::createEntity("Player"));
        player->get<TransformComponent>()->position = {0, 0, 2};

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
        platform->get<TransformComponent>()->position = {0, 0, -0.4};

        sphere = Scene::createEntity("Sphere");
        sphere->get<TransformComponent>()->position = {1, 2, 1};
        sphere->get<TransformComponent>()->scale = {0.3f, 0.3f, 0.3f};

        cube = Scene::createEntity("Cube");
        cube->get<TransformComponent>()->position = {3, 2, 1.5};
        cube->get<TransformComponent>()->scale = {0.3f, 0.3f, 0.3f};

        materialStanic.setShader(&AssetManager::get()->shaderPhong);
    }

    void initAfterEngine() override
    {
        modelLoaderBackpack.load("assets/models/backpack/backpack.obj");
        if(useMap) modelLoaderMap.load("assets/sponza/scene.gltf");

        auto* pPointLight = light->add<PointLightComponent>();
        pPointLight->light.ambient = Vec3f(1.0f, 1.0f, 1.0f)
        pPointLight->light.diffuse = Vec3f(0.2, 0.2, 0.2);
        pPointLight->light.specular = Vec3f(0.5, 0.5, 0.5);

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
        //light->get<TransformComponent>()->position = Vec3f(sin(Time::getTime() / 2) * 2, cos(Time::getTime() / 2) * 2, 2 + sin(Time::getTime() / 5));
        light->get<TransformComponent>()->position = Vec3f(sin(Time::getTime() / 100) * 2, cos(Time::getTime() / 100) * 2, 2 + sin(Time::getTime() / 5));

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

        ///// -------
        //platform->get<ModelComponent>()->model.pMaterials[0]->set("u_Light.position", light->get<TransformComponent>()->position);
        //stanic->get<ModelComponent>()->model.pMaterials[0]->set("u_Light.position", light->get<TransformComponent>()->position);
        //sphere->get<ModelComponent>()->model.pMaterials[0]->set("u_Light.position", light->get<TransformComponent>()->position);
        //cube->get<ModelComponent>()->model.pMaterials[0]->set("u_Light.position", light->get<TransformComponent>()->position);
        //if(useMap) map->get<ModelComponent>()->model.pMaterials[0]->set("u_Light.position", light->get<TransformComponent>()->position);

        //platform->get<ModelComponent>()->model.pMaterials[0]->set("u_Light.ambient", Vec3f(0.2, 0.2, 0.2));
        //stanic->get<ModelComponent>()->model.pMaterials[0]->set("u_Light.ambient", Vec3f(0.2f, 0.2f, 0.2f));
        //sphere->get<ModelComponent>()->model.pMaterials[0]->set("u_Light.ambient", Vec3f(0.2f, 0.2f, 0.2f));
        //cube->get<ModelComponent>()->model.pMaterials[0]->set("u_Light.ambient", Vec3f(0.2f, 0.2f, 0.2f));
        //if(useMap) map->get<ModelComponent>()->model.pMaterials[0]->set("u_Light.ambient", Vec3f(0.2f, 0.2f, 0.2f));

        //platform->get<ModelComponent>()->model.pMaterials[0]->set("u_Light.diffuse", Vec3f(0.5, 0.5, 0.5));
        //stanic->get<ModelComponent>()->model.pMaterials[0]->set("u_Light.diffuse", Vec3f(0.5, 0.5, 0.5));
        //sphere->get<ModelComponent>()->model.pMaterials[0]->set("u_Light.diffuse", Vec3f(0.5, 0.5, 0.5));
        //cube->get<ModelComponent>()->model.pMaterials[0]->set("u_Light.diffuse", Vec3f(0.5, 0.5, 0.5));
        //if(useMap) map->get<ModelComponent>()->model.pMaterials[0]->set("u_Light.diffuse", Vec3f(0.5, 0.5, 0.5));

        //platform->get<ModelComponent>()->model.pMaterials[0]->set("u_Light.specular", Vec3f(1.0f, 1.0f, 1.0f));
        //stanic->get<ModelComponent>()->model.pMaterials[0]->set("u_Light.specular", Vec3f(1.0f, 1.0f, 1.0f));
        //sphere->get<ModelComponent>()->model.pMaterials[0]->set("u_Light.specular", Vec3f(1.0f, 1.0f, 1.0f));
        //cube->get<ModelComponent>()->model.pMaterials[0]->set("u_Light.specular", Vec3f(1.0f, 1.0f, 1.0f));
        //if(useMap) map->get<ModelComponent>()->model.pMaterials[0]->set("u_Light.specular", Vec3f(1.0f, 1.0f, 1.0f));
        ///// -------

        platform->get<ModelComponent>()->model.pMaterials[0]->set("u_ViewPosition", player->get<TransformComponent>()->position);
        stanic->get<ModelComponent>()->model.pMaterials[0]->set("u_ViewPosition", player->get<TransformComponent>()->position);
        sphere->get<ModelComponent>()->model.pMaterials[0]->set("u_ViewPosition", player->get<TransformComponent>()->position);
        cube->get<ModelComponent>()->model.pMaterials[0]->set("u_ViewPosition", player->get<TransformComponent>()->position);
        if(useMap) map->get<ModelComponent>()->model.pMaterials[0]->set("u_ViewPosition", player->get<TransformComponent>()->position);

        //stanicTexture.setUp("../resources/textures/stene.png");
        //stanicTexture.bind(0);
        //beloTexture.setUp("../resources/textures/belo.png");
    }

};

Application* createApplication()
{
    return new Sandbox();
}