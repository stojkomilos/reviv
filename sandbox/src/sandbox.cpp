#include<reviv.h>

Entity *player, *camera, *stanic, *light, *platform, *sphere, *cube;
Entity* map;

Entity *secondSphere;

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
            map->add<ModelComponent>(&modelLoaderMap, &RenderManager::get()->deffered.geometryPassShader);
        }

        platform = Scene::createEntity("Platform");
        platform->get<TransformComponent>()->scale = {50, 14, 0.4};
        platform->add<ModelComponent>(&AssetManager::get()->modelLoaderCube, &RenderManager::get()->deffered.geometryPassShader);
        platform->get<ModelComponent>()->model.pMaterials[0]->set("u_Diffuse", Vec3f(1, 1, 0));
        platform->get<ModelComponent>()->model.pMaterials[0]->set("u_Specular", 0.5f);
        auto* pPlatformPhysical = platform->add<PhysicalComponent>();
        pPlatformPhysical->physical.gravity = 0.0;
        //auto* pPlatformCollider = platform->add<ColliderBoxComponent>();

        sphere = Scene::createEntity("Sphere");
        sphere->get<TransformComponent>()->position = {10, 6, 20};
        sphere->add<ModelComponent>(&AssetManager::get()->modelLoaderSphere, &RenderManager::get()->deffered.geometryPassShader);
        sphere->get<ModelComponent>()->model.pMaterials[0]->set("u_Diffuse", Vec3f(1, 0, 0));
        sphere->get<ModelComponent>()->model.pMaterials[0]->set("u_Specular", 0.2f);
        auto* pSpherePhysical = sphere->add<PhysicalComponent>();
        auto* pSphereCollider = sphere->add<ColliderSphereComponent>();
        //pSpherePhysical->physical.velocity = Vec3f{7, -3, 1.2};
        pSpherePhysical->physical.velocity = Vec3f{0, 0, 1.2};

        secondSphere = Scene::createEntity("SecondSphere");
        secondSphere->get<TransformComponent>()->position = {10, 6, 10};
        secondSphere->add<ModelComponent>(&AssetManager::get()->modelLoaderSphere, &RenderManager::get()->deffered.geometryPassShader);
        secondSphere->get<ModelComponent>()->model.pMaterials[0]->set("u_Diffuse", Vec3f(1, 0, 0));
        secondSphere->get<ModelComponent>()->model.pMaterials[0]->set("u_Specular", 0.2f);
        auto* pSecondSpherePhysical = secondSphere->add<PhysicalComponent>();
        auto* pSecondSphereCollider = secondSphere->add<ColliderSphereComponent>();
        pSecondSpherePhysical->physical.gravity = 0.0f;

        cube = Scene::createEntity("Cube");
        cube->get<TransformComponent>()->position = {20, 0, 10};
        cube->get<TransformComponent>()->scale = {1, 1, 1};
        cube->add<ModelComponent>(&AssetManager::get()->modelLoaderCube, &RenderManager::get()->deffered.geometryPassShader);
        cube->get<ModelComponent>()->model.pMaterials[0]->set("u_Diffuse", Vec3f(0, 0, 1));
        cube->get<ModelComponent>()->model.pMaterials[0]->set("u_Specular", 0.2f);
        cube->add<PhysicalComponent>();
        auto* pPhysicalCube = &cube->get<PhysicalComponent>()->physical;
        

    }

    void onUpdate() override
    {

    }
};

Application* createApplication()
{
    return new Sandbox();
}