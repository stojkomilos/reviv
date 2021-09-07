#include<reviv.h>

Entity *player, *camera, *stanic, *light, *platform, *sphere;
Entity* map;
Entity* room;

Entity *prvo;
Entity *drugo;
Entity *debugLine;

ModelLoader modelLoaderBackpack, modelLoaderMap;

Entity *dirLight;

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

        debugLine = Scene::createEntity("DebugLine");
        debugLine->add<ModelComponent>(&AssetManager::get()->modelLoaderCube, &RenderManager::get()->shaderMonochroma);

        dirLight = Scene::createEntity("dirLight");
        dirLight->get<TransformComponent>()->position = {0, 0, 20};
        dirLight->add<DirectionalLightComponent>();
        dirLight->get<TransformComponent>()->rotation.pitch = degreesToRadians(-90);

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
        //auto* pPlatformCollider = platform->add<ColliderMeshComponent>();
        pPlatformPhysical->physical.gravity = 0.0;
        //pPlatformCollider->collider.pTransformComponent = platform->get<TransformComponent>();
        //pPlatformCollider->collider.pMesh = platform->get<ModelComponent>()->model.pMeshes[0];

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

        srand(1);

        prvo = Scene::createEntity("prvo");
        prvo->get<TransformComponent>()->position = {13, 2, 5.1};
        prvo->get<TransformComponent>()->rotation = Vec3f(rand() / 100.f, rand() / 100.f, rand() / 100.f);
        prvo->add<ModelComponent>(&AssetManager::get()->modelLoaderDodik, &RenderManager::get()->deffered.geometryPassShader);
        prvo->get<ModelComponent>()->model.pMaterials[0]->set("u_Diffuse", Vec3f(0, 0, 1));
        prvo->get<ModelComponent>()->model.pMaterials[0]->set("u_Specular", 0.2f);
        prvo->add<PhysicalComponent>();
        auto* pPhysicalPrvo = &prvo->get<PhysicalComponent>()->physical;
        pPhysicalPrvo->gravity = 0.0;
        auto* pColliderPrvo = prvo->add<ColliderMeshComponent>();
        pColliderPrvo->collider.pMesh = prvo->get<ModelComponent>()->model.pMeshes[0];

        drugo = Scene::createEntity("Drugo");
        drugo->get<TransformComponent>()->position = {13, 0, 3};
        drugo->get<TransformComponent>()->rotation = Vec3f(rand() / 100.f, rand() / 100.f, rand() / 100.f);
        drugo->add<ModelComponent>(&AssetManager::get()->modelLoaderHexagon, &RenderManager::get()->deffered.geometryPassShader);
        drugo->get<ModelComponent>()->model.pMaterials[0]->set("u_Diffuse", Vec3f(0, 0, 1));
        drugo->get<ModelComponent>()->model.pMaterials[0]->set("u_Specular", 0.2f);
        drugo->add<PhysicalComponent>();
        auto* pPhysicalDrugo = &drugo->get<PhysicalComponent>()->physical;
        auto* pColliderDrugo = drugo->add<ColliderMeshComponent>();
        pPhysicalDrugo->gravity = 0.0;
        pColliderDrugo->collider.pMesh = drugo->get<ModelComponent>()->model.pMeshes[0]; // TODO: automate this somehow
    }

    void onUpdate() override
    {
        auto* trans = prvo->get<TransformComponent>();

        float drugoSpeed = 1.f;
        if(Input::isKeyPressed(RV_KEY_UP))
            trans->position += Time::getDelta() * Vec3f(0, 0, 1) * drugoSpeed;
        if(Input::isKeyPressed(RV_KEY_DOWN))
            trans->position += Time::getDelta() * Vec3f(0, 0, -1) * drugoSpeed;

        if(Input::isKeyPressed(RV_KEY_LEFT))
            trans->position += Time::getDelta() * Vec3f(0, 1, 0) * drugoSpeed;
        if(Input::isKeyPressed(RV_KEY_RIGHT))
            trans->position += Time::getDelta() * Vec3f(0, -1, 0) * drugoSpeed;

        //trans->position += Time::getDelta() * Vec3f(0, 0, -1) * 0.1;

/*
        prvo->get<TransformComponent>()->rotation.yaw = Time::getTime();
        prvo->get<TransformComponent>()->rotation.roll = Time::getTime() * 3.1;
        prvo->get<TransformComponent>()->rotation.pitch = Time::getTime() * 0.3 + 0.7;

        drugo->get<TransformComponent>()->rotation.yaw = Time::getTime() * 0.7;
        drugo->get<TransformComponent>()->rotation.roll = Time::getTime() * 1.3 + 0.333;
        drugo->get<TransformComponent>()->rotation.pitch = Time::getTime() * 1.8 + 0.2;
*/



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

    }
};

Application* createApplication()
{
    return new Sandbox();
}