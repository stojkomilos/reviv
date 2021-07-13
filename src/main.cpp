#include <reviv.h>

using namespace mat;

RenderManager gRenderManager;
PhysicsManager gPhysicsManager;

ModelLoader sphere;
ModelLoader cube;

int gGameLoopCounter = 0;
const int gMapSize = 50;

using std::cin; using std::cout; using std::endl;

Entity* stanic;
Entity* player;

int main(){ 
			
	std::cout << "START\n";

    stanic = Scene::createEntity("Stanic");

    player = Scene::createEntity("Player");
    Scene::setPlayerEntity(player);

    auto* camera = Scene::createEntity("Camera");
    Scene::setCameraEntity(camera);

    stanic->add<PositionComponent>();
    auto* stanicPos = stanic->get<PositionComponent>();

    *stanicPos = Vec3f(3, 3, 3);

    auto* stanicTrans = stanic->add<TransformComponent>();
    cout << "NOV SIZE: " << stanic->components.size() << endl;

    auto stanicModel = stanic->add<ModelLoaderComponent>();
    stanicModel->modelLoader.LoadModel("../resources/models/cube.obj");

    auto* playerPos = Scene::getPlayerEntity()->add<PositionComponent>();
    *playerPos = PositionComponent(Vec3f(1, 1, 1));
    auto* playerRot = Scene::getPlayerEntity()->add<RotationComponent>();
    *playerRot = RotationComponent(Vec3f(0, 0, 0));

    auto* cameraCamera = Scene::getCameraEntity()->add<CameraComponent>();
    auto* cameraPos = Scene::getCameraEntity()->add<PositionComponent>();
    auto* cameraRot = Scene::getCameraEntity()->add<RotationComponent>();


    gRenderManager.init();

	float time0 = (float)glfwGetTime();
	auto timeEnd = std::chrono::high_resolution_clock::now();

	gRenderManager.renderHitbox = false;

	while(true)
    {

		//processInput(gRenderManager.window.pWindow); // TODO: apstraktuj ulaz

        gPhysicsManager.update();
		gRenderManager.render();

        if(gGameLoopCounter % 1000 == 0)
            log(*stanic);
        //log( (*Scene::getEntityList())[0] );
        //logSpecificUniform(stanic->get<MaterialComponent>()->material, "u_Model");
        //log(stanic->get<PositionComponent>()->position);

		gGameLoopCounter++;

	}
	
	gRenderManager.shutdown();

	return 0;
}