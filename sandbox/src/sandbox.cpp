#include<reviv.h>

Entity *player, *camera, *stanic, *light, *platform, *sphere, *cube;
Entity* map;

Entity* sun;
Entity* secondSun;

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
        glEnable(GL_FRAMEBUFFER_SRGB);

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
        platform->get<TransformComponent>()->scale = {50, 14, 0.4};
        platform->add<ModelComponent>(&AssetManager::get()->modelLoaderCube, &RenderManager::getInstance()->deffered.geometryPassShader);
        platform->get<ModelComponent>()->model.pMaterials[0]->set("u_Diffuse", Vec3f(1, 1, 0));
        platform->get<ModelComponent>()->model.pMaterials[0]->set("u_Specular", 0.5f);

        sphere = Scene::createEntity("Sphere");
        sphere->get<TransformComponent>()->position = {10, 6, 10};
        sphere->add<ModelComponent>(&AssetManager::get()->modelLoaderSphere, &RenderManager::getInstance()->deffered.geometryPassShader);
        sphere->get<ModelComponent>()->model.pMaterials[0]->set("u_Diffuse", Vec3f(1, 0, 0));
        sphere->get<ModelComponent>()->model.pMaterials[0]->set("u_Specular", 0.2f);

        cube = Scene::createEntity("Cube");
        cube->get<TransformComponent>()->position = {20, 0, 10};
        cube->get<TransformComponent>()->scale = {1, 1, 1};
        cube->add<ModelComponent>(&AssetManager::get()->modelLoaderCube, &RenderManager::getInstance()->deffered.geometryPassShader);
        cube->get<ModelComponent>()->model.pMaterials[0]->set("u_Diffuse", Vec3f(0, 0, 1));
        cube->get<ModelComponent>()->model.pMaterials[0]->set("u_Specular", 0.2f);

        cube->add<PhysicalComponent>();
        auto* pPhysical = &cube->get<PhysicalComponent>()->physical;

    }

    void onUpdate() override
    {

    }
};

Application* createApplication()
{
    return new Sandbox();
}