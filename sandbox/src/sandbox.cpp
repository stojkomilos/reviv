#include<reviv.h>

Entity *stanic, *pointLight, *platform, *sphere, *cube;
Entity* map;
Entity* room;
Entity* dirLight;

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

        platform = Scene::createEntity("Platform");
        //platform->get<TransformComponent>()->scale = {7, 14, 0.4};
        platform->get<TransformComponent>()->scale = {50, 14, 0.4};
        platform->add<ModelComponent>(&AssetManager::get()->modelLoaderCube, &RenderManager::getInstance()->deffered.geometryPassShader);
        platform->get<ModelComponent>()->model.pMaterials[0]->set("u_Diffuse", Vec3f(1, 1, 0));
        platform->get<ModelComponent>()->model.pMaterials[0]->set("u_Specular", 0.5f);

        sphere = Scene::createEntity("Sphere");
        sphere->get<TransformComponent>()->position = {1, 6, 2};
        sphere->get<TransformComponent>()->scale *= 0.65f;
        sphere->add<ModelComponent>(&AssetManager::get()->modelLoaderSphere, &RenderManager::getInstance()->deffered.geometryPassShader);
        sphere->get<ModelComponent>()->model.pMaterials[0]->set("u_Diffuse", Vec3f(1, 0, 0));
        sphere->get<ModelComponent>()->model.pMaterials[0]->set("u_Specular", 0.2f);

        cube = Scene::createEntity("Cube");
        cube->get<TransformComponent>()->position = {0, 0, 0.8 + cube->get<TransformComponent>()->scale.a[2] + 4};
        cube->get<TransformComponent>()->scale *= 0.3;
        cube->add<ModelComponent>(&AssetManager::get()->modelLoaderCube, &RenderManager::getInstance()->deffered.geometryPassShader);
        cube->get<ModelComponent>()->model.pMaterials[0]->set("u_Diffuse", Vec3f(0, 0, 1));
        cube->get<ModelComponent>()->model.pMaterials[0]->set("u_Specular", 0.2f);

        dirLight = Scene::createEntity("dirLight");
        auto* pDirComp = dirLight->add<DirectionalLightComponent>();

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

        //set<TransformComponent>()->position = Vec3f(cos(Time::getTime() * 1) * 4, sin(Time::getTime() * 1) * 4, 3.5);
        cube->get<TransformComponent>()->position = Vec3f(cos(Time::getTime() * 1) * 4, sin(Time::getTime() * 1) * 4, 7);
        sphere->get<TransformComponent>()->position = Vec3f(sin(Time::getTime() * 1) * 4, cos(Time::getTime() * 1) * 4, 20.f);

        TransformComponent* plTrans = Scene::getPlayerEntity()->get<TransformComponent>();
        if(Input::isKeyPressed(RV_KEY_1))
            plTrans->rotation = lookAtGetRotation(plTrans->position, plTrans->position + Vec3f(1, 0, 0));
        if(Input::isKeyPressed(RV_KEY_2))
            plTrans->rotation = lookAtGetRotation(plTrans->position, plTrans->position + Vec3f(-1, 0, 0));
        if(Input::isKeyPressed(RV_KEY_3))
            plTrans->rotation = lookAtGetRotation(plTrans->position, plTrans->position + Vec3f(0, 1, 0));
        if(Input::isKeyPressed(RV_KEY_4))
            plTrans->rotation = lookAtGetRotation(plTrans->position, plTrans->position + Vec3f(0, -1, 0));
        if(Input::isKeyPressed(RV_KEY_5))
            plTrans->rotation = lookAtGetRotation(plTrans->position, plTrans->position + Vec3f(0, 0, 1));
        if(Input::isKeyPressed(RV_KEY_6))
            plTrans->rotation = lookAtGetRotation(plTrans->position, plTrans->position + Vec3f(0, 0, -1));

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