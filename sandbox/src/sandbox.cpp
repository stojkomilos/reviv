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

    void initSnake()
    {
        float distance = 2.3f;
        int nrParts = 1;
        float beta = 0.01f;

        Entity *pOld = nullptr;

        for(int i=0; i<nrParts; i++)
        {
            std::string entityName = "snake";
            entityName = entityName + (char)(i + '0');
            Entity* pThis = Scene::createEntity(entityName);
            pThis->add<ModelComponent>(&AssetManager::get()->modelLoaderSphere, &RenderManager::get()->shaderDefferedGeometry);
            pThis->add<PhysicalComponent>();
            pThis->add<ColliderSphereComponent>();
            //pThis->get<PhysicalComponent>()->physical.gravity = 0.f;

            Vec3 color;
            color.randomize(0, 1);
            pThis->get<ModelComponent>()->model.pMaterials[0]->set("u_Diffuse", color);

            if(pOld != nullptr)
            {
                auto constraint = Scene::addConstraintDistance(pThis, pOld, distance);
                constraint->beta = beta;

                *pThis->get<TransformComponent>()->getPositionPtr() = 
                    pOld->get<TransformComponent>()->getPosition() 
                    + normalize(Vec3(distance, distance, 0)) * distance;
            }
            else {
                //auto first = platform->get<TransformComponent>()->getPosition();
                //auto second = Vec3(2.f, 0, platform->get<TransformComponent>()->getScale().get(2, 0) / 2.f);
                //auto third  = Vec3(2.f, 0, pThis->get<TransformComponent>()->getScale().get(2, 0) + 10.f);
                //auto fourth = first + second + third;
                //*pThis->get<TransformComponent>()->getPositionPtr() =  fourth;

                *pThis->get<TransformComponent>()->getPositionPtr() = 
                    platform->get<TransformComponent>()->getPosition() 
                    + Vec3(0.f, 0, platform->get<TransformComponent>()->getScale().get(2, 0) / 2.f)
                    + Vec3(6.f, 0, pThis->get<TransformComponent>()->getScale().get(2, 0) + 10.f);
            }
            pOld = pThis;
        }
    }

    void initAfterEngine() override
    {
        //Scene::get()->gravity = 1.f;

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
            map->get<TransformComponent>()->setScale({0.005f, 0.005f, 0.005f});
            *map->get<TransformComponent>()->getRotationPtr()->getPtr(0, 0) = degreesToRadians(90);
            modelLoaderMap.load("assets/sponza/scene.gltf");
            map->add<ModelComponent>(&modelLoaderMap, &RenderManager::get()->shaderDefferedGeometry);
        }

        platform = Scene::createEntity("Platform");
        platform->get<TransformComponent>()->setScale({100, 100, 0.4});
        platform->add<ModelComponent>(&AssetManager::get()->modelLoaderCube, &RenderManager::get()->shaderDefferedGeometry);
        platform->get<ModelComponent>()->model.pMaterials[0]->set("u_Diffuse", Vec3(1, 1, 0));
        platform->get<ModelComponent>()->model.pMaterials[0]->set("u_Specular", 0.5f);
        platform->add<ColliderMeshComponent>()->collider.pMesh = platform->get<ModelComponent>()->model.pMeshes[0];
        *platform->get<TransformComponent>()->getPositionPtr()->getPtr(2, 0) += platform->get<TransformComponent>()->getScale().get(2, 0) / 2.f;
        //platform->get<ColliderMeshComponent>()->collider.pTransformComponent = platform->get<TransformComponent>();

        platform->add<PhysicalComponent>();
        platform->get<PhysicalComponent>()->physical.fixedTranslation = true;
        platform->get<PhysicalComponent>()->physical.fixedRotation = true;
        platform->get<PhysicalComponent>()->physical.gravity = 0.f;


        sphere = Scene::createEntity("Sphere");
        *sphere->get<TransformComponent>()->getPositionPtr() = {10, -10, 5};
        sphere->add<ModelComponent>(&AssetManager::get()->modelLoaderSphere, &RenderManager::get()->shaderBlend);
        sphere->get<ModelComponent>()->model.pMaterials[0]->set("u_Color", Vec4(0, 1, 0, 0.7));
        auto* pSpherePhysical = sphere->add<PhysicalComponent>();
        pSpherePhysical->physical.gravity = 0.0;

        srand(1);

        prvo = Scene::createEntity("prvo");
        *prvo->get<TransformComponent>()->getPositionPtr() = {3, 0, 10};
        prvo->get<TransformComponent>()->getRotationPtr()->randomize(0, 2*3.14f);
        //prvo->add<ModelComponent>(&AssetManager::get()->modelLoaderDodik, &RenderManager::get()->shaderDefferedGeometry);
        prvo->add<ModelComponent>(&AssetManager::get()->modelLoaderCube, &RenderManager::get()->shaderDefferedGeometry);
        prvo->get<ModelComponent>()->model.pMaterials[0]->set("u_Diffuse", Vec3(1, 0, 0));
        prvo->get<ModelComponent>()->model.pMaterials[0]->set("u_Specular", 0.2f);
        prvo->add<PhysicalComponent>();
        auto* pPhysicalPrvo = &prvo->get<PhysicalComponent>()->physical;
        auto* pColliderPrvo = prvo->add<ColliderMeshComponent>();
        pColliderPrvo->collider.pMesh = prvo->get<ModelComponent>()->model.pMeshes[0];
        //pPhysicalPrvo->gravity = 0.f;

        drugo = Scene::createEntity("drugo");
        *drugo->get<TransformComponent>()->getPositionPtr() = prvo->get<TransformComponent>()->getPosition() + Vec3(0.7, 0.5, 3);
        *drugo->get<TransformComponent>()->getRotationPtr() = Vec3(rand() / 100.f, rand() / 100.f, rand() / 100.f);
        //drugo->add<ModelComponent>(&AssetManager::get()->modelLoaderHexagon, &RenderManager::get()->shaderDefferedGeometry);
        drugo->add<ModelComponent>(&AssetManager::get()->modelLoaderCube, &RenderManager::get()->shaderDefferedGeometry);
        drugo->get<ModelComponent>()->model.pMaterials[0]->set("u_Diffuse", Vec3(0, 1, 0));
        drugo->get<ModelComponent>()->model.pMaterials[0]->set("u_Specular", 0.2f);
        drugo->add<PhysicalComponent>();
        auto* pPhysicalDrugo = &drugo->get<PhysicalComponent>()->physical;
        auto* pColliderDrugo = drugo->add<ColliderMeshComponent>();
        pColliderDrugo->collider.pMesh = drugo->get<ModelComponent>()->model.pMeshes[0]; // TODO: automate this somehow

        initSnake();


        //prvo->valid = false;
        drugo->valid = false;
    }

    void onUpdate() override
    {
        auto* trans = prvo->get<TransformComponent>();
        //auto* trans = Scene::getEntity("snake0")->get<TransformComponent>();

        cout << "prvo Total: " << prvo->get<PhysicalComponent>()->physical.getKineticEnergy() + prvo->get<PhysicalComponent>()->physical.getMass() * prvo->get<TransformComponent>()->getPosition().get(2, 0) * prvo->get<PhysicalComponent>()->physical.gravity << endl;

        float drugoSpeed = 1.f;
        if(Input::get()->isKeyPressed(RV_KEY_UP))
            *trans->getPositionPtr() += Time::get()->getDelta() * Vec3(0, 0, 1) * drugoSpeed;
        if(Input::get()->isKeyPressed(RV_KEY_DOWN))
            *trans->getPositionPtr() += Time::get()->getDelta() * Vec3(0, 0, -1) * drugoSpeed;

        if(Input::get()->isKeyPressed(RV_KEY_LEFT))
            *trans->getPositionPtr() += Time::get()->getDelta() * Vec3(0, 1, 0) * drugoSpeed;
        if(Input::get()->isKeyPressed(RV_KEY_RIGHT))
            *trans->getPositionPtr() += Time::get()->getDelta() * Vec3(0, -1, 0) * drugoSpeed;

        if(Input::get()->isKeyPressed(RV_KEY_U))
        {
            //Scene::getEntity("snake0")->get<PhysicalComponent>()->physical.force = {0, 0, 1000.f};
            Scene::getEntity("snake0")->get<PhysicalComponent>()->physical.velocity = {0, 0, 3.f};
        }

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