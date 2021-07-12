#include <reviv.h>

using namespace mat;

RenderManager gRenderManager;
PhysicsManager gPhysicsManager;

ModelLoader sphere;
ModelLoader cube;

int gGameLoopCounter = 0;
const int gMapSize = 50;

using std::cin; using std::cout; using std::endl;
//Entity* stanic;

Entity* stanic;
Entity* player;
int main(){ 
			
	std::cout << "START\n";

    Scene::createEntity("Stanic");
    //cout << "pPlayerPre:" << Scene::getInstance().pPlayerEntity << endl;

    player = Scene::createEntity("Player");
    Scene::setPlayerEntity(player);
    //cout << "player->entityName: " << player->entityName << endl;

    //cout << "pPlayerPosle:" << Scene::getInstance().pPlayerEntity << endl;
    auto* camera = Scene::createEntity("Camera");
    Scene::setCameraEntity(camera);
    //Scene::createEntity("Kurac");


    stanic = &Scene::getInstance().entityList[0];
    //stanic = nullptr;

    stanic->add<PositionComponent>();
    auto* stanicPos = stanic->get<PositionComponent>();

    *stanicPos = Vec3f(3, 3, 3);
    //cout << "EVO POS: ";
    //log(*stanic->get<PositionComponent>());
    //cout << "NOV SIZE: " << stanic->components.size() << endl;


    //cout << "NOV SIZE: " << stanic->components.size() << endl;
    //Scene::createEntity("Camera");
    //cout << "NOV SIZE: " << stanic->components.size() << endl;
    //Scene::setPlayerEntity(a);
    //Scene::setCameraEntity(b);

    //Scene::setPlayerEntity(Scene::createEntity("Player"));
    //Scene::setCameraEntity(Scene::createEntity("Camera"));

    ////

    auto* stanicTrans = stanic->add<TransformComponent>();
    cout << "NOV SIZE: " << stanic->components.size() << endl;
    //*stanicTrans = Mat4();

    //log(*stanic->get<TransformComponent>());

    auto stanicModel = stanic->add<ModelLoaderComponent>();
    stanicModel->modelLoader.LoadModel("../resources/models/cube.obj");

    auto* playerPos = Scene::getPlayerEntity(1)->add<PositionComponent>();
    *playerPos = PositionComponent(Vec3f(1, 1, 1));
    auto* playerRot = Scene::getPlayerEntity(2)->add<RotationComponent>();
    *playerRot = RotationComponent(Vec3f(0, 0, 0));
	////

    auto* cameraCamera = Scene::getCameraEntity()->add<CameraComponent>();
    auto* cameraPos = Scene::getCameraEntity()->add<PositionComponent>();
    auto* cameraRot = Scene::getCameraEntity()->add<RotationComponent>();


	if (gRenderManager.init(1280, 720) == -1) {
		return -1;
	}

	float time0 = (float)glfwGetTime();
	auto timeEnd = std::chrono::high_resolution_clock::now();

	gRenderManager.renderHitbox = false;

	while(true)
    {

		processInput(gRenderManager.window);

		/*
		gPhysicsManager.deltat = ((float)glfwGetTime() - time0);
		time0 = (float)glfwGetTime();
		if (1.0 / 75.0 - gPhysicsManager.deltat > 0) {
          printf("main.cpp, red : 63, linux prelaz namesti usleep! takodje DWORD ne valja\n");
			usleep( (DWORD) ((1.f / 75.f - gPhysicsManager.deltat) * 1000.f) ); TODO
          usleep(100);
			gPhysicsManager.deltat = 1.f / 75.f;
		}
		if (gPhysicsManager.deltat > 0.7f)
			gPhysicsManager.deltat = 1 / 75.f;
		gPhysicsManager.deltat *= 1;
		*/

		if (gGameLoopCounter % 1 == 0)
		{
			//cout << "radi\n";
			/*
			std::cout << "fps = " << 1.0f / gPhysicsManager.deltat << std::endl;
			std::cout << "x = " << gPhysicsManager.player.position.x << " y = " << gPhysicsManager.player.position.y << " z = " << gPhysicsManager.player.position.z << std::endl;
			
			std::cout << "pitch = " << gPhysicsManager.player.pitch << " yaw = " << gPhysicsManager.player.yaw << std::endl;
			std::cout << "vx = " << gPhysicsManager.player.velocity.x << " vy = " << gPhysicsManager.player.velocity.y << " vz = " << gPhysicsManager.player.velocity.z << std::endl;
			std::cout << "standingOnSurface = " << gPhysicsManager.player.standingOnSurface << std::endl;
			std::cout << "\n\n\n\n";
			*/
		}

        gPhysicsManager.update(); //TODO: destruktor radi ovde
		gRenderManager.render();
		gGameLoopCounter++;

	}
	
	gRenderManager.shutdown();
	return 0;
}