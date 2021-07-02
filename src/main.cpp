#include <reviv.h>
#include "core/entity.h"

RenderManager gRenderManager;
PhysicsManager gPhysicsManager;
FileManager gFileManager;
SimulationManager gSimulationManager;
RandomManager gRandomManager;
ModelLoader sphere;
ModelLoader cube;

void processInput(GLFWwindow* window);
int gGameLoopCounter = 0;
const int gMapSize = 50;

mat::vec3 gPointLightPosition;

mat::vec3 gPlayerInitialPosition = mat::vec3(0, 10, 0);

using namespace mat;
using std::cin; using std::cout; using std::endl;

int main(){ 
			
	std::cout << "START\n";

	gRandomManager.setUpPerlin(gMapSize);
    
	sphere.LoadModel("../resources/models/sphere.obj");
	cube.LoadModel  ("../resources/models/cube.obj");

	Entity stanic;
	LightComponent jedanSvetlo;
	LightComponent dvaSvetlo;
	RenderableComponent tri;
	stanic.components.push_back(&jedanSvetlo);
	stanic.components.push_back(&dvaSvetlo);
	stanic.components.push_back(&tri);

	cout << "stanicLight: " << stanic.hasComponent("Light") << endl;
	cout << "stanicRenderable: " << stanic.hasComponent("Renderable") << endl;
	
	//gPhysicsManager.startUp(gMapSize, 50, gMapSize, 9.81f);
	if (gRenderManager.startUp(1280, 720) == -1)
	{
		return -1;
	}

	//gSimulationManager.setUp();

	float time0 = (float)glfwGetTime();
	auto timeEnd = std::chrono::high_resolution_clock::now();
   
	gPhysicsManager.player.creativeMode = false;
	gPhysicsManager.player.speed = 5;
	gRenderManager.renderHitbox = false;

	while(!glfwWindowShouldClose(gRenderManager.window)){

		processInput(gRenderManager.window);

		gPhysicsManager.deltat = ((float)glfwGetTime() - time0);
		time0 = (float)glfwGetTime();
		if (1.0 / 75.0 - gPhysicsManager.deltat > 0) {
            //printf("main.cpp, red : 63, linux prelaz namesti usleep! takodje DWORD ne valja\n");
			//usleep( (DWORD) ((1.f / 75.f - gPhysicsManager.deltat) * 1000.f) ); TODO
            usleep(100);
			gPhysicsManager.deltat = 1.f / 75.f;
		}
		if (gPhysicsManager.deltat > 0.7f)
			gPhysicsManager.deltat = 1 / 75.f;
		gPhysicsManager.deltat *= 1;

		if (gGameLoopCounter % 5 == 0)
		{
			std::cout << "fps = " << 1.0f / gPhysicsManager.deltat << std::endl;
			std::cout << "x = " << gPhysicsManager.player.position.x << " y = " << gPhysicsManager.player.position.y << " z = " << gPhysicsManager.player.position.z << std::endl;
			
			std::cout << "pitch = " << gPhysicsManager.player.pitch << " yaw = " << gPhysicsManager.player.yaw << std::endl;
			std::cout << "vx = " << gPhysicsManager.player.velocity.x << " vy = " << gPhysicsManager.player.velocity.y << " vz = " << gPhysicsManager.player.velocity.z << std::endl;
			std::cout << "standingOnSurface = " << gPhysicsManager.player.standingOnSurface << std::endl;
			std::cout << "\n\n\n\n";
			
			std::cout << "\n";
		}

		if (gGameLoopCounter % 7500 == 0 and gGameLoopCounter != 0) ///precesto
		{
			//gFileManager.saveCurrentWorld();
		}
	   
		//gSimulationManager.doShit();
		gPhysicsManager.doShit();
		gRenderManager.render();
		gGameLoopCounter++;

	}
	
	gRenderManager.shutDown();
	delete[] gPhysicsManager.kocke;
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}