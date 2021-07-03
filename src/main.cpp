#include <reviv.h>

using namespace mat;

RenderManager gRenderManager;

ModelLoader sphere;
ModelLoader cube;

//void processInput(GLFWwindow* window);
int gGameLoopCounter = 0;
const int gMapSize = 50;

Entity gStanic;
Entity gStojko;
Entity gCamera;
Entity gPlayer;

//Entity gEntityList[4];
std::vector<Entity*> gEntityList;

using std::cin; using std::cout; using std::endl;

int main(){ 
			
//	gEntityList[0] = &gStanic;
//	gEntityList[1] = &gStojko;
//	gEntityList[2] = &gCamera;
//	gEntityList[3] = &gPlayer;

	std::cout << "START\n";

    ////
	ModelLoader cubeModel;
	gStanic.addComponent<ModelLoader>(&cubeModel);
	(*gStanic.getComponent<ModelLoader>()).LoadModel("../resources/models/cube.obj");

	PositionComponent stanicPosition(Vec3f(3, 3, 3));
	gStanic.addComponent<PositionComponent>(&stanicPosition);

	TransformComponent trans = Mat4();
	gStanic.addComponent<TransformComponent>(&trans);

	PositionComponent pos(Vec3f(1, 1, 1));
	gPlayer.addComponent<PositionComponent>(&pos);
	RotationComponent rot(Vec3f(0, 0, 0));
	gPlayer.addComponent<RotationComponent>(&rot);
	////

	PerspectiveCameraComponent kam;
	gCamera.addComponent<PerspectiveCameraComponent>(&kam);

	ModelLoader sphereModel;
	sphereModel.LoadModel("../resources/models/sphere.obj");


	if (gRenderManager.startUp(1280, 720) == -1)
	{
		return -1;
	}

	float time0 = (float)glfwGetTime();
	auto timeEnd = std::chrono::high_resolution_clock::now();

	gRenderManager.renderHitbox = false;

	while(!glfwWindowShouldClose(gRenderManager.window)){

		//processInput(gRenderManager.window);

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
			cout << "radi\n";
			/*
			std::cout << "fps = " << 1.0f / gPhysicsManager.deltat << std::endl;
			std::cout << "x = " << gPhysicsManager.player.position.x << " y = " << gPhysicsManager.player.position.y << " z = " << gPhysicsManager.player.position.z << std::endl;
			
			std::cout << "pitch = " << gPhysicsManager.player.pitch << " yaw = " << gPhysicsManager.player.yaw << std::endl;
			std::cout << "vx = " << gPhysicsManager.player.velocity.x << " vy = " << gPhysicsManager.player.velocity.y << " vz = " << gPhysicsManager.player.velocity.z << std::endl;
			std::cout << "standingOnSurface = " << gPhysicsManager.player.standingOnSurface << std::endl;
			std::cout << "\n\n\n\n";
			*/
		}

		gRenderManager.render();
		gGameLoopCounter++;

	}
	
	gRenderManager.shutDown();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}