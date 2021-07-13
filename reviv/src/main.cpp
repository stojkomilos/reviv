#include <reviv.h>

using namespace mat;

RenderManager gRenderManager;
PhysicsManager gPhysicsManager;
Window gWindow;

ModelLoader sphere;
ModelLoader cube;

int gGameLoopCounter = 0;
const int gMapSize = 50;

using std::cin; using std::cout; using std::endl;

Entity* stanic;
Entity* player;

/*
int main(){ 

    std::cout << "START\n";

    stanic = Scene::createEntity("Stanic");

    player = Scene::createEntity("Player");
    Scene::setPlayerEntity(player);

    auto* camera = Scene::createEntity("Camera");
    Scene::setCameraEntity(camera);

    stanic->add<PositionComponent>();
    auto* stanicPos = &stanic->get<PositionComponent>()->position;

    *stanicPos = Vec3f(3, 3, 3);

    auto* stanicTrans = &stanic->add<TransformComponent>()->transform;
    cout << "NOV SIZE: " << stanic->components.size() << endl;

    auto* stanicModel = &stanic->add<ModelLoaderComponent>()->modelLoader;
    stanicModel->LoadModel("../resources/models/cube.obj");

    auto* playerPos = &Scene::getPlayerEntity()->add<PositionComponent>()->position;
    *playerPos = PositionComponent(Vec3f(1, 1, 1));
    auto* playerRot = &Scene::getPlayerEntity()->add<RotationComponent>()->rotation;
    *playerRot = RotationComponent(Vec3f(0, 0, 0));

    auto* cameraCamera = &Scene::getCameraEntity()->add<CameraComponent>()->camera;
    auto* cameraPos = &Scene::getCameraEntity()->add<PositionComponent>()->position;
    auto* cameraRot = &Scene::getCameraEntity()->add<RotationComponent>()->rotation;


    float time0 = (float)glfwGetTime();
    auto timeEnd = std::chrono::high_resolution_clock::now();

    gRenderManager.renderHitbox = false;

    gWindow.init();
    gRenderManager.init();

    while(true)
    {

        //processInput(gRenderManager.window.pWindow); // TODO: apstraktuj ulaz

        gPhysicsManager.update();
        gRenderManager.render();
        gWindow.onUpdate();

        gGameLoopCounter++;

    }

    gRenderManager.shutdown();

    return 0;
}
*/