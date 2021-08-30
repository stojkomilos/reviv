#include<reviv.h>

Entity *player, *camera, *stanic, *light, *platform, *sphere, *cube;
Entity* map;

Entity *secondSphere;
Entity *secondCube;

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
        //auto* pSphereCollider = sphere->add<ColliderSphereComponent>();
        //pSpherePhysical->physical.velocity = Vec3f{7, -3, 1.2};
        pSpherePhysical->physical.velocity = Vec3f{0, 0, 1.2};

        secondSphere = Scene::createEntity("SecondSphere");
        secondSphere->get<TransformComponent>()->position = {10, 6, 100};
        secondSphere->add<ModelComponent>(&AssetManager::get()->modelLoaderSphere, &RenderManager::get()->deffered.geometryPassShader);
        secondSphere->get<ModelComponent>()->model.pMaterials[0]->set("u_Diffuse", Vec3f(1, 0, 0));
        secondSphere->get<ModelComponent>()->model.pMaterials[0]->set("u_Specular", 0.2f);
        auto* pSecondSpherePhysical = secondSphere->add<PhysicalComponent>();
        //auto* pSecondSphereCollider = secondSphere->add<ColliderSphereComponent>();
        pSecondSpherePhysical->physical.gravity = 0.0f;

        srand(1);

        cube = Scene::createEntity("Cube");
        cube->get<TransformComponent>()->position = {10, 0, 3};
        cube->get<TransformComponent>()->rotation = Vec3f(rand() / 100.f, rand() / 100.f, rand() / 100.f);
        cube->get<TransformComponent>()->scale = {1.7, 2, 1};
        cube->add<ModelComponent>(&AssetManager::get()->modelLoaderCube, &RenderManager::get()->deffered.geometryPassShader);
        cube->get<ModelComponent>()->model.pMaterials[0]->set("u_Diffuse", Vec3f(0, 0, 1));
        cube->get<ModelComponent>()->model.pMaterials[0]->set("u_Specular", 0.2f);
        cube->add<PhysicalComponent>();
        auto* pPhysicalCube = &cube->get<PhysicalComponent>()->physical;
        auto* pColliderCube = cube->add<ColliderMeshComponent>();
        pPhysicalCube->gravity = 0.0;
        pColliderCube->collider.pMesh = cube->get<ModelComponent>()->model.pMeshes[0]; // TODO: automate this somehow

        secondCube = Scene::createEntity("SecondCube");
        secondCube->get<TransformComponent>()->position = {13, 0, 3.8};
        secondCube->get<TransformComponent>()->rotation = Vec3f(rand() / 100.f, rand() / 100.f, rand() / 100.f);
        secondCube->get<TransformComponent>()->scale = {1, 0.3, 3};
        secondCube->add<ModelComponent>(&AssetManager::get()->modelLoaderCube, &RenderManager::get()->deffered.geometryPassShader);
        secondCube->get<ModelComponent>()->model.pMaterials[0]->set("u_Diffuse", Vec3f(0, 0, 1));
        secondCube->get<ModelComponent>()->model.pMaterials[0]->set("u_Specular", 0.2f);
        secondCube->add<PhysicalComponent>();
        auto* pPhysicalSecondCube = &secondCube->get<PhysicalComponent>()->physical;
        pPhysicalSecondCube->gravity = 0.0;
        auto* pColliderSecondCube = secondCube->add<ColliderMeshComponent>();
        pColliderSecondCube->collider.pMesh = secondCube->get<ModelComponent>()->model.pMeshes[0];
    }

    void onUpdate() override
    {
        auto* trans = secondCube->get<TransformComponent>();

        if(Input::isKeyPressed(RV_KEY_UP))
            trans->position += Time::getDelta() * Vec3f(0, 0, 1);
        if(Input::isKeyPressed(RV_KEY_DOWN))
            trans->position += Time::getDelta() * Vec3f(0, 0, -1);

        if(Input::isKeyPressed(RV_KEY_LEFT))
            trans->position += Time::getDelta() * Vec3f(0, 1, 0);
        if(Input::isKeyPressed(RV_KEY_RIGHT))
            trans->position += Time::getDelta() * Vec3f(0, -1, 0);

        //Rotation firstRot = cube->get<TransformComponent>()->rotation;
        //Rotation secondRot = secondCube->get<TransformComponent>()->rotation;

        //cube->get<TransformComponent>()->rotation.yaw += Time::getDelta();
        trans->rotation.yaw += Time::getDelta();
        cout << "delta: " << Time::getDelta() << endl;
        //cube->get<TransformComponent>()->rotation.roll += Time::GetDelta();

        //secondCube->get<TransformComponent>()->rotation.yaw -= Time::getDelta();




    }
};

Application* createApplication()
{
    return new Sandbox();
}