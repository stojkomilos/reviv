//#include<windows.h>
#include<iostream>
#include<stdlib.h>
#include<time.h>
#include<chrono>
#include<assert.h>
#include<unistd.h>
#include<sstream>

#include<filesystem>

#include <reviv.h>

RenderManager gRenderManager;
PhysicsManager gPhysicsManager;
FileManager gFileManager;
SimulationManager gSimulationManager;
RandomManager gRandomManager;
ModelLoader sphere; //neka ne bude globalna, render batch??
ModelLoader cube;

void processInput(GLFWwindow* window);
int gGameLoopCounter = 0;
const int gMapSize = 50;
mat::vec3 gPointLightPosition;
int main(){ ///WTF zasto je fakin levo pozitivna Z(mozda ok)
			//modelloader 0 indexed
			//crash kada gMapSize = 2
			//jedan vao moze imati vise vbo, koristiti za instanced rendering
	std::cout << "START\n";

	std::cout << "Starting setting up perlin noise\n";
	gRandomManager.setUpPerlin(gMapSize); /// moze kroz 5, ili 10, zavisi od generacije
	std::cout << "Completed setting up perlin noise\n"; 

    
    /// -> nov deo 2021
    //std::string proj_dir  = "/home/milos/reviv2/";
    //std::string sphereStr = "resources/models/sphere.obj";
    //std::string cubeStr   = "resources/models/cube.obj";

	using std::filesystem::current_path;

	char tmp[256];
	getcwd(tmp, 256);
	std::cout << "Current directory: " << tmp << std::endl;

	sphere.LoadModel("../resources/models/sphere.obj");
	cube.LoadModel  ("../resources/models/sphere.obj");
    /// -> kraj novog dela 2021

	gPhysicsManager.startUp(gMapSize, 50, gMapSize, 9.81f);
	if (gRenderManager.startUp(1280, 720) == -1) {
		assert(false);
		return -1;
	}

	gSimulationManager.setUp();

	float time0 = (float)glfwGetTime();
	auto timeEnd = std::chrono::high_resolution_clock::now();
   
	gPhysicsManager.player.creativeMode = false;
	gPhysicsManager.player.speed = 5;
	gRenderManager.renderHitbox = false;

    int prolazMs = 0;
	while(!glfwWindowShouldClose(gRenderManager.window)){

		processInput(gRenderManager.window);

		gPointLightPosition = gPhysicsManager.player.position;
		gPointLightPosition.x += 5 * sin(glfwGetTime() * 2.4);
		gPointLightPosition.y += 0 ;
		gPointLightPosition.z += 5 * cos(glfwGetTime() * 2.4);

		gPhysicsManager.deltat = ((float)glfwGetTime() - time0);
		time0 = (float)glfwGetTime();
		if (1.0 / 75.0 - gPhysicsManager.deltat > 0) {
            printf("main.cpp, red : 63, linux prelaz namesti usleep! takodje DWORD ne valja\n");
			//usleep( (DWORD) ((1.f / 75.f - gPhysicsManager.deltat) * 1000.f) );
            usleep(100);
			gPhysicsManager.deltat = 1.f / 75.f;
		}
		if (gPhysicsManager.deltat > 0.7f)
			gPhysicsManager.deltat = 1 / 75.f;
		gPhysicsManager.deltat *= 1;

		if (gGameLoopCounter % 5 == 0) {
			std::cout << "fps = " << 1.0f / gPhysicsManager.deltat << std::endl;
			std::cout << "x = " << gPhysicsManager.player.position.x << " y = " << gPhysicsManager.player.position.y << " z = " << gPhysicsManager.player.position.z << std::endl;
			//std::cout << "pitch = " << gPhysicsManager.player.pitch << " yaw = " << gPhysicsManager.player.yaw << std::endl;
			//std::cout << "vx = " << gPhysicsManager.player.velocity.x << " vy = " << gPhysicsManager.player.velocity.y << " vz = " << gPhysicsManager.player.velocity.z << std::endl;
			//std::cout << "standingOnSurface = " << gPhysicsManager.player.standingOnSurface << std::endl;
			//std::cout << "\n\n\n\n";
			std::cout << "\n";
		}

		if (gGameLoopCounter % 7500 == 0 and gGameLoopCounter != 0) { ///precesto
			gFileManager.saveCurrentWorld();
		}
	   
		gSimulationManager.doShit();
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