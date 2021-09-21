#include<reviv.h>

Entity *player, *camera, *stanic, *light, *platform, *sphere;
Entity* map;
Entity* room;

Entity *prvo;
Entity *drugo;
Entity *debugLine;

ModelLoader modelLoaderMap;

Entity *dirLight;
Entity *transWindow;

bool useMap = false;

Perlin2D perlin;

Texture2D textureWindow;

class Sandbox : public Application
{
public:

    void initAfterEngine() override
    {

        player = Scene::getPlayerEntity();
        *player->get<TransformComponent>()->getPositionPtr() = {0, 0, 3};

        debugLine = Scene::createEntity("DebugLine");
        debugLine->add<ModelComponent>(&AssetManager::get()->modelLoaderCube, &RenderManager::get()->shaderMonochroma);
        debugLine->get<ModelComponent>()->model.pMaterials[0]->set("u_Color", Vec3(0, 1, 0));

        dirLight = Scene::createEntity("dirLight");
        *dirLight->get<TransformComponent>()->getPositionPtr() = {0, 0, 20};
        dirLight->add<DirectionalLightComponent>();
        *dirLight->get<TransformComponent>()->getRotationPtr()->getPtr(1, 0) = degreesToRadians(-90);

        transWindow = Scene::createEntity("Window");
        *transWindow->get<TransformComponent>()->getPositionPtr() = {5, 0, 4};
        transWindow->add<ModelComponent>(&AssetManager::get()->modelLoaderQuad2D, &RenderManager::get()->shaderBlend);
        transWindow->get<ModelComponent>()->model.pMaterials[0]->set("u_Color", Vec4(0, 0, 1, 0.3));

        if(useMap)
        {
            map = Scene::createEntity("Sponza");
            *map->get<TransformComponent>()->getPositionPtr()->getPtr(2, 0) = 2;
            *map->get<TransformComponent>()->getScalePtr() = {0.005f, 0.005f, 0.005f};
            *map->get<TransformComponent>()->getRotationPtr()->getPtr(0, 0) = degreesToRadians(90);
            modelLoaderMap.load("assets/sponza/scene.gltf");
            map->add<ModelComponent>(&modelLoaderMap, &RenderManager::get()->shaderDefferedGeometry);
        }

        platform = Scene::createEntity("Platform");
        *platform->get<TransformComponent>()->getScalePtr() = {50, 14, 0.4};
        platform->add<ModelComponent>(&AssetManager::get()->modelLoaderCube, &RenderManager::get()->shaderDefferedGeometry);
        platform->get<ModelComponent>()->model.pMaterials[0]->set("u_Diffuse", Vec3(1, 1, 0));
        platform->get<ModelComponent>()->model.pMaterials[0]->set("u_Specular", 0.5f);
        platform->add<ColliderMeshComponent>()->collider.pMesh = platform->get<ModelComponent>()->model.pMeshes[0];
        //platform->get<ColliderMeshComponent>()->collider.pTransformComponent = platform->get<TransformComponent>();

        platform->add<PhysicalComponent>();
        //platform->get<PhysicalComponent>()->physical.fixedTranslation = true;
        //platform->get<PhysicalComponent>()->physical.fixedRotation = true;
        platform->get<PhysicalComponent>()->physical.gravity = 0.f;


        sphere = Scene::createEntity("Sphere");
        *sphere->get<TransformComponent>()->getPositionPtr() = {10, -10, 5};
        sphere->add<ModelComponent>(&AssetManager::get()->modelLoaderSphere, &RenderManager::get()->shaderBlend);
        sphere->get<ModelComponent>()->model.pMaterials[0]->set("u_Color", Vec4(0, 1, 0, 0.7));
        auto* pSpherePhysical = sphere->add<PhysicalComponent>();
        pSpherePhysical->physical.gravity = 0.0;

        srand(1);

        prvo = Scene::createEntity("prvo");
        *prvo->get<TransformComponent>()->getPositionPtr() = {3, 0, 3};
        *prvo->get<TransformComponent>()->getRotationPtr() = Vec3(rand() / 100.f, rand() / 100.f, rand() / 100.f);
        //prvo->add<ModelComponent>(&AssetManager::get()->modelLoaderDodik, &RenderManager::get()->shaderDefferedGeometry);
        prvo->add<ModelComponent>(&AssetManager::get()->modelLoaderCube, &RenderManager::get()->shaderDefferedGeometry);
        prvo->get<ModelComponent>()->model.pMaterials[0]->set("u_Diffuse", Vec3(1, 0, 0));
        prvo->get<ModelComponent>()->model.pMaterials[0]->set("u_Specular", 0.2f);
        prvo->add<PhysicalComponent>();
        auto* pPhysicalPrvo = &prvo->get<PhysicalComponent>()->physical;
        pPhysicalPrvo->gravity = 0.5f;
        auto* pColliderPrvo = prvo->add<ColliderMeshComponent>();
        pColliderPrvo->collider.pMesh = prvo->get<ModelComponent>()->model.pMeshes[0];

        drugo = Scene::createEntity("drugo");
        //drugo->get<TransformComponent>()->position = {13, 3, 6};
        *drugo->get<TransformComponent>()->getPositionPtr() = prvo->get<TransformComponent>()->getPosition() + Vec3(0, 0.3, 2);
        *drugo->get<TransformComponent>()->getRotationPtr() = Vec3(rand() / 100.f, rand() / 100.f, rand() / 100.f);
        //drugo->add<ModelComponent>(&AssetManager::get()->modelLoaderHexagon, &RenderManager::get()->shaderDefferedGeometry);
        drugo->add<ModelComponent>(&AssetManager::get()->modelLoaderCube, &RenderManager::get()->shaderDefferedGeometry);
        drugo->get<ModelComponent>()->model.pMaterials[0]->set("u_Diffuse", Vec3(0, 1, 0));
        drugo->get<ModelComponent>()->model.pMaterials[0]->set("u_Specular", 0.2f);
        drugo->add<PhysicalComponent>();
        auto* pPhysicalDrugo = &drugo->get<PhysicalComponent>()->physical;
        auto* pColliderDrugo = drugo->add<ColliderMeshComponent>();
        //pPhysicalDrugo->gravity = 0.1f;
        pColliderDrugo->collider.pMesh = drugo->get<ModelComponent>()->model.pMeshes[0]; // TODO: automate this somehow
    }

    void onUpdate() override
    {

        auto* trans = prvo->get<TransformComponent>();

        prvo->get<ModelComponent>()->model.pMaterials[0]->set("u_Specular", 0.2f);

        float drugoSpeed = 1.f;
        if(Input::get()->isKeyPressed(RV_KEY_UP))
            *trans->getPositionPtr() += Time::get()->getDelta() * Vec3(0, 0, 1) * drugoSpeed;
        if(Input::get()->isKeyPressed(RV_KEY_DOWN))
            *trans->getPositionPtr() += Time::get()->getDelta() * Vec3(0, 0, -1) * drugoSpeed;

        if(Input::get()->isKeyPressed(RV_KEY_LEFT))
            *trans->getPositionPtr() += Time::get()->getDelta() * Vec3(0, 1, 0) * drugoSpeed;
        if(Input::get()->isKeyPressed(RV_KEY_RIGHT))
            *trans->getPositionPtr() += Time::get()->getDelta() * Vec3(0, -1, 0) * drugoSpeed;

        //trans->position += Time::getDelta() * Vec3(0, 0, -1) * 0.1;

/*
        prvo->get<TransformComponent>()->rotation.yaw = Time::getTime();
        prvo->get<TransformComponent>()->rotation.roll = Time::getTime() * 3.1;
        prvo->get<TransformComponent>()->rotation.pitch = Time::getTime() * 0.3 + 0.7;

        drugo->get<TransformComponent>()->rotation.yaw = Time::getTime() * 0.7;
        drugo->get<TransformComponent>()->rotation.roll = Time::getTime() * 1.3 + 0.333;
        drugo->get<TransformComponent>()->rotation.pitch = Time::getTime() * 1.8 + 0.2;
*/

/*
        TransformComponent* plTrans = Scene::getPlayerEntity()->get<TransformComponent>();
        if(Input::get()->isKeyPressed(RV_KEY_1))
            plTrans->rotation = lookAtGetRotation(plTrans->position, plTrans->position + Vec3(1, 0, 0));
        if(Input::get()->isKeyPressed(RV_KEY_2))
            plTrans->rotation = lookAtGetRotation(plTrans->position, plTrans->position + Vec3(-1, 0, 0));
        if(Input::get()->isKeyPressed(RV_KEY_3))
            plTrans->rotation = lookAtGetRotation(plTrans->position, plTrans->position + Vec3(0, 1, 0));
        if(Input::get()->isKeyPressed(RV_KEY_4))
            plTrans->rotation = lookAtGetRotation(plTrans->position, plTrans->position + Vec3(0, -1, 0));
        if(Input::get()->isKeyPressed(RV_KEY_5))
            plTrans->rotation = lookAtGetRotation(plTrans->position, plTrans->position + Vec3(0, 0, 1));
        if(Input::get()->isKeyPressed(RV_KEY_6))
            plTrans->rotation = lookAtGetRotation(plTrans->position, plTrans->position + Vec3(0, 0, -1));
*/

    }
};

Application* createApplication()
{
    return new Sandbox();
}