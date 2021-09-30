#include<reviv.h>

Entity *player, *camera, *stanic, *light, *platform;
Entity* map;
Entity* room;

Entity *prvo;
Entity *drugo;
Entity *debugLine;

ModelLoader modelLoaderMap;

Entity *dirLight;
Entity *transWindow;

bool useMap = false;

class Sandbox : public Application
{
public:

    void initSnake()
    {
        float distance = 2.3f;
        int nrParts = 0;
        float beta = 0.1f;
        float betaSpring = 0.001f;

        Entity *pOld = nullptr;

        for(int i=0; i<nrParts; i++)
        {
            std::string entityName = "snake";
            entityName = entityName + (char)(i + '0');
            Entity* pThis = Scene::createEntity(entityName);
            pThis->add<ModelComponent>(&AssetManager::get()->modelLoaderSphere, &RenderManager::get()->shaderDefferedGeometry);
            pThis->addRigidbodyComponent(RigidbodyShape::SPHERE, ColliderShape::SPHERE);
            //pThis->add<ColliderSphereComponent>();
            //pThis->get<RigidbodyComponent>()->rigidbody.gravity = 0.f;

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

/*
                if(i>1)
                {
                    std::string name = "snake";
                    name += (char)(i + '0' - 2);
                    auto constraintSpring = Scene::addConstraintDistance(pThis, Scene::getEntity(name), distance * 2.f);
                    constraintSpring->beta = betaSpring;
                }
*/
            }
            else {

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
        //platform->add<ColliderMeshComponent>()->collider.pMesh = platform->get<ModelComponent>()->model.pMeshes[0];
        *platform->get<TransformComponent>()->getPositionPtr()->getPtr(2, 0) += platform->get<TransformComponent>()->getScale().get(2, 0) / 2.f;
        //platform->get<ColliderMeshComponent>()->collider.pTransformComponent = platform->get<TransformComponent>();

        RigidbodyComponent* pRigidbodyComponent = platform->addRigidbodyComponent(RigidbodyShape::BOX, ColliderShape::MESH);
        // platform->get<RigidbodyComponent>()->rigidbody.fixedTranslation = true;
        // platform->get<RigidbodyComponent>()->rigidbody.fixedRotation = true;
        platform->get<RigidbodyComponent>()->rigidbody.gravity = 0.f;

        srand(1);

        prvo = Scene::createEntity("prvo");
        *prvo->get<TransformComponent>()->getPositionPtr() = {10, 3, 3};
        prvo->get<TransformComponent>()->getRotationPtr()->randomize(0, 2*3.14f);
        //prvo->add<ModelComponent>(&AssetManager::get()->modelLoaderDodik, &RenderManager::get()->shaderDefferedGeometry);
        prvo->add<ModelComponent>(&AssetManager::get()->modelLoaderSphere, &RenderManager::get()->shaderDefferedGeometry);
        prvo->get<ModelComponent>()->model.pMaterials[0]->set("u_Diffuse", Vec3(1, 0, 0));
        prvo->get<ModelComponent>()->model.pMaterials[0]->set("u_Specular", 0.2f);
        prvo->addRigidbodyComponent(RigidbodyShape::SPHERE, ColliderShape::SPHERE);
        // prvo->get<RigidbodyComponent>()->rigidbody.gravity = 0;

        drugo = Scene::createEntity("drugo");
        //*drugo->get<TransformComponent>()->getPositionPtr() = prvo->get<TransformComponent>()->getPosition() + Vec3(0.7, 0.5, 5);
        *drugo->get<TransformComponent>()->getPositionPtr() = prvo->get<TransformComponent>()->getPosition() + Vec3(0, 0, 2.5);
        *drugo->get<TransformComponent>()->getRotationPtr() = Vec3(rand() / 100.f, rand() / 100.f, rand() / 100.f);
        //drugo->add<ModelComponent>(&AssetManager::get()->modelLoaderHexagon, &RenderManager::get()->shaderDefferedGeometry);
        drugo->add<ModelComponent>(&AssetManager::get()->modelLoaderCube, &RenderManager::get()->shaderDefferedGeometry);
        drugo->get<ModelComponent>()->model.pMaterials[0]->set("u_Diffuse", Vec3(0, 1, 0));
        drugo->get<ModelComponent>()->model.pMaterials[0]->set("u_Specular", 0.2f);
        drugo->addRigidbodyComponent(RigidbodyShape::BOX, ColliderShape::MESH);
        // drugo->get<RigidbodyComponent>()->rigidbody.gravity = 0.f;

        initSnake();

        //auto help = prvo->get<TransformComponent>()->getPosition();
        //*prvo->get<TransformComponent>()->getPositionPtr() = drugo->get<TransformComponent>()->getPosition();
        //*drugo->get<TransformComponent>()->getPositionPtr() = help;

        prvo->valid = false; // sphere
        //drugo->valid = false; // cube
    }

    void onUpdate() override
    {
        auto arrowController = &drugo->get<RigidbodyComponent>()->rigidbody.force;
        //auto arrowController = drugo->get<TransformComponent>()->getPositionPtr();
        //auto arrowController = &prvo->get<RigidbodyComponent>()->rigidbody.velocity;
        //float arrowControllerConst = Time::get()->getDelta() * 3.f;
        float arrowControllerConst = 30.f;
        
        //if(PhysicsManager::get()->constraintsCollision.size() > 0)
        //    log(drugo->get<RigidbodyComponent>()->rigidbody);

        //cout << "total: " << Scene::getEntity("snake0")->get<RigidbodyComponent>()->rigidbody.getTotalEnergy() << endl;
        //cout << "kinetic: " << Scene::getEntity("snake0")->get<RigidbodyComponent>()->rigidbody.getKineticEnergy() << endl;
        //cout << "potential: " << Scene::getEntity("snake0")->get<RigidbodyComponent>()->rigidbody.getPotentialEnergy() << endl;
        //cout << "MY position: " << endl;
        //log(Scene::getEntity("snake0")->get<RigidbodyComponent>()->rigidbody.getTransform()->getPosition());

        //log(*prvo->get<RigidbodyComponent>());

        float drugoSpeed = 1.f;
        if(Input::get()->isKeyPressed(RV_KEY_UP))
            *arrowController += Vec3(0, 0, arrowControllerConst);
        if(Input::get()->isKeyPressed(RV_KEY_DOWN))
            *arrowController += Vec3(0, 0, -arrowControllerConst);

        if(Input::get()->isKeyPressed(RV_KEY_LEFT))
            *arrowController += Vec3(0, arrowControllerConst, 0);
        if(Input::get()->isKeyPressed(RV_KEY_RIGHT))
            *arrowController += Vec3(0, -arrowControllerConst, 0);

        if(Input::get()->isKeyPressed(RV_KEY_U))
        {
            Scene::getEntity("snake0")->get<RigidbodyComponent>()->rigidbody.force = {0, 0, 100.f};
            //Scene::getEntity("snake0")->get<RigidbodyComponent>()->rigidbody.velocity = {0, 0, 5.f};
        }

        if(Input::get()->isKeyPressed(RV_KEY_P))
            Time::get()->timeRatio = 0.1f;
        else Time::get()->timeRatio = 1.f;
        //Time::get()->timeRatio = 0.1f;

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