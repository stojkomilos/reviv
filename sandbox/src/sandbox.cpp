#include<reviv.h>

Entity *stanic, *pointLight, *platform, *sphere, *cube;
Entity* map;

Entity *player, *camera;

ModelLoader modelLoaderBackpack, modelLoaderMap;

bool useMap = false;

Perlin2D perlin;

class Sandbox : public Application
{
public:

    void initBeforeEngine() override
    {
    }

    void initAfterEngine() override
    {

        //GameStuffManager::get()->weather.init("WeatherSun", 12);

        pointLight = Scene::createEntity("PointLight");
        pointLight->add<ModelComponent>(&AssetManager::get()->modelLoaderSphere, &RenderManager::getInstance()->deffered.geometryPassShader);
        pointLight->add<PointLightComponent>();
        pointLight->get<TransformComponent>()->position = Vec3f{0, 0, 6};
        pointLight->get<TransformComponent>()->rotation.pitch = degreesToRadians(-90);
        pointLight->get<TransformComponent>()->scale *= 0.3f;
        pointLight->get<ModelComponent>()->model.pMaterials[0]->set("u_Diffuse", Vec3f(1.f, 0.f, 0.3f));
        pointLight->get<ModelComponent>()->model.pMaterials[0]->set("u_Specular", 0.3f);

        pointLight->get<PointLightComponent>()->light.enableShadowMap();
        pointLight->get<PointLightComponent>()->light.getShadowMap()->renderDistance = 25.f;
        pointLight->get<PointLightComponent>()->light.getShadowMap()->nearRenderDistance = 0.1f;
        
        /*
        int voxelMapSize = 11;
        perlin.init(voxelMapSize, voxelMapSize);
        for(int i=-voxelMapSize/2; i<voxelMapSize/2; i++)
        {
            for(int j=-voxelMapSize/2; j<voxelMapSize/2; j++)
            {
                Entity* entity = Scene::createEntity("voxel");
                Vec2f tempVec;
                tempVec.a[0] = i * 0.3;
                tempVec.a[1] = j * 0.3;
                float perlinResult = perlin.get(tempVec);
                cout << "perlin: " << perlinResult << endl;

                entity->get<TransformComponent>()->position.a[0] = i;
                entity->get<TransformComponent>()->position.a[1] = j;
                entity->get<TransformComponent>()->position.a[2] = 5 + perlinResult * 10;
                
                entity->get<TransformComponent>()->scale = {0.5, 0.5, 0.5};

                entity->add<ModelComponent>(&AssetManager::get()->modelLoaderCube, &RenderManager::getInstance()->deffered.geometryPassShader);
            }
        }
        */


        player = Scene::getPlayerEntity();
        player->get<TransformComponent>()->position = {0, 0, 3};

        if(useMap)
        {
            map = Scene::createEntity("Sponza");
            map->get<TransformComponent>()->position.a[2] = 2;
            map->get<TransformComponent>()->scale = {0.005f, 0.005f, 0.005f};
            map->get<TransformComponent>()->rotation.roll = degreesToRadians(90);
            modelLoaderMap.load("assets/sponza/scene.gltf");
            map->add<ModelComponent>(&modelLoaderMap, &RenderManager::getInstance()->deffered.geometryPassShader);
        }

        //stanic = Scene::createEntity("Stanic");
        //stanic->get<TransformComponent>()->position = {1, 3, 5};
        //modelLoaderBackpack.load("assets/models/backpack/backpack.obj");
        //stanic->add<ModelComponent>(&modelLoaderBackpack, &RenderManager::getInstance()->deffered.geometryPassShader);
        //stanic->get<ModelComponent>()->model.pMaterials[0]->set("u_Diffuse", Vec3f(1, 0, 1));
        //stanic->get<ModelComponent>()->model.pMaterials[0]->set("u_Specular", 0.5f);

        platform = Scene::createEntity("Platform");
        //platform->get<TransformComponent>()->scale = {7, 14, 0.4};
        platform->get<TransformComponent>()->scale = {50, 14, 0.4};
        platform->add<ModelComponent>(&AssetManager::get()->modelLoaderCube, &RenderManager::getInstance()->deffered.geometryPassShader);
        platform->get<ModelComponent>()->model.pMaterials[0]->set("u_Diffuse", Vec3f(1, 1, 0));
        platform->get<ModelComponent>()->model.pMaterials[0]->set("u_Specular", 0.5f);

        sphere = Scene::createEntity("Sphere");
        sphere->get<TransformComponent>()->position = {1, 6, 2};
        //sphere->get<TransformComponent>()->scale = {0.3f, 0.3f, 0.3f};
        sphere->add<ModelComponent>(&AssetManager::get()->modelLoaderSphere, &RenderManager::getInstance()->deffered.geometryPassShader);
        sphere->get<ModelComponent>()->model.pMaterials[0]->set("u_Diffuse", Vec3f(1, 0, 0));
        sphere->get<ModelComponent>()->model.pMaterials[0]->set("u_Specular", 0.2f);

        cube = Scene::createEntity("Cube");
        cube->get<TransformComponent>()->position = {0, 0, 0.8 + cube->get<TransformComponent>()->scale.a[2] + 4};
        cube->get<TransformComponent>()->scale = {1, 1, 1};
        cube->add<ModelComponent>(&AssetManager::get()->modelLoaderCube, &RenderManager::getInstance()->deffered.geometryPassShader);
        cube->get<ModelComponent>()->model.pMaterials[0]->set("u_Diffuse", Vec3f(0, 0, 1));
        cube->get<ModelComponent>()->model.pMaterials[0]->set("u_Specular", 0.2f);
    }

    void onUpdate() override
    {
        for(auto it = Scene::getEntityList()->begin(); it != Scene::getEntityList()->end(); it++)
        {
            if(it->entityName != "voxel")
                continue;
            float help = (sin(Time::getTime() / 2) + 1) / 2;
            it->get<TransformComponent>()->scale = Vec3f(help/2, help/2, help/2);
        }
        //secondSun.setSunTimeOfDay(Time::getTime());

        //light->get<TransformComponent>()->position = Vec3f(sin(Time::getTime() / 2) * 2, cos(Time::getTime() / 2) * 3, 2 + sin(Time::getTime() / 5));

        //GameStuffManager::get()->weather.setSunTimeOfDay(Time::getTime());

        Scene::getPlayerEntity()->get<TransformComponent>()->rotation = lookAtGetRotation(Scene::getPlayerEntity()->get<TransformComponent>()->position, cube->get<TransformComponent>()->position);

        cube->get<TransformComponent>()->position = Vec3f(0, sin(Time::getTime()) * 3, 3.5);
        //cube->get<TransformComponent>()->rotation.yaw = 1;
        //cube->get<TransformComponent>()->rotation.pitch = 1;
        //cube->get<TransformComponent>()->rotation.roll = 1;
        //cube->get<TransformComponent>()->rotation.yaw = Time::getTime() * 7;
        //cube->get<TransformComponent>()->rotation.pitch = Time::getTime() * 11;
        //cube->get<TransformComponent>()->rotation.roll = Time::getTime() * 3;


        if(Time::isOneSecond())
        {
            cout << "FPS: " << 1 / Time::getDelta() << endl;
            cout << "Position: ";
            log(Scene::getPlayerEntity()->get<TransformComponent>()->position);
            cout << "Rotation: ";
            log(Scene::getPlayerEntity()->get<TransformComponent>()->rotation);
        }
    }
};

Application* createApplication()
{
    return new Sandbox();
}