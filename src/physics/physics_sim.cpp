#include"physics_sim.h"

extern PhysicsManager gPhysicsManager;
extern int gGameLoopCounter;
SimulationManager::SimulationManager() {
	nrCollidables = 0;
	g = totalEnergy = 0;
}
SimulationManager::Shape::Shape() {
	mass = type = 0;
}
SimulationManager::Ball::Ball() {
	radius = 0;
}
void SimulationManager::setUp() {

	//g = gPhysicsManager.g;
	g = 0;
	nrCollidables = 0;
	for (int i = 0; i < nrCollidables; i++){
		generateBall(&balls[i]);
		bool isItTooClose = true;
		while (isItTooClose) {
			isItTooClose = false;
			for (int j = i - 1; j >= 0; j--) {
				if (mat::module(mat::subtract(balls[i].position, balls[j].position)) < balls[i].radius + balls[j].radius + 1) {
					generateBall(&balls[i]);
					isItTooClose = true;
				}
			}
		}
	}
	/*if (nrCollidables == 2) {
		balls[0].acceleration = mat::vec3(0, -g, 0);
		balls[0].mass =  1;
		balls[0].radius = 1;

		balls[1].acceleration = mat::vec3(0, -g, 0);
		balls[1].mass = 10;
		balls[1].radius = 1;

		balls[0].position = mat::vec3(0.5, gPhysicsManager.mapSizeY + 2, +2);
		balls[1].position = mat::vec3(0, gPhysicsManager.mapSizeY + 2, -2);

		balls[0].velocity = mat::vec3(0, 0, -1);
		balls[1].velocity = mat::vec3(0, 0, 3);
	}*/
}
void SimulationManager::generateBall(Ball* a) {
	
	int speedRange = 10;
	a->position = mat::vec3((float)(rand() % gPhysicsManager.mapSizeX - gPhysicsManager.mapSizeX / 2), (float)(rand() % gPhysicsManager.mapSizeX),(float)(rand() % gPhysicsManager.mapSizeZ - gPhysicsManager.mapSizeZ / 2));
	a->velocity = mat::vec3((float)(rand() % speedRange), (float)(rand() % speedRange), (float)(rand() % speedRange));
	a->acceleration = mat::vec3(0.f, -g, 0.f);
	//a->position = mat::vec3(0, 10, 2);
	//a->velocity = mat::vec3(0, 0, 0);
	//a->acceleration = mat::vec3(0, 0, 0);

	a->mass = 1;
	a->radius = 0.1f;
}
void SimulationManager::collide(Ball *a, Ball *b) {

	Ball bFromA0, bFromA1;
	bFromA0 = *a;
	Ball aFromA = *a;
	bFromA0.position = mat::subtract(b->position, a->position);
	bFromA0.prevPosition = mat::subtract(b->prevPosition, a->prevPosition);
	bFromA0.velocity = mat::subtract(b->velocity, a->velocity);

	bFromA1 = bFromA0;

	
	if (mat::module(bFromA0.position) < bFromA0.radius + a->radius) {
		
		//std::cout << "SUDAR DVE LOPTE\n";
		mat::vec3 normal;
		normal = bFromA0.position; //neprecizna normala
		normal = mat::normalise(normal);
		bFromA1.velocity = mat::multiplyScalar(normal, mat::dot(bFromA0.velocity, normal) * (b->mass - a->mass) / (b->mass + a->mass));
		mat::vec3 paralell = mat::subtract(bFromA0.velocity, mat::multiplyScalar(normal, mat::dot(bFromA0.velocity, normal)));
		bFromA1.velocity = mat::add(bFromA1.velocity, paralell);
		b->velocity = mat::add(bFromA1.velocity, a->velocity);
		//b->velocity = bFromA1.velocity;
		
		aFromA.velocity = mat::multiplyScalar(normal, (float)(mat::dot(bFromA0.velocity, normal) * 2.0f * b->mass / (a->mass + b->mass)));
		mat::vec3 pom = a->velocity;
		a->velocity = mat::add(aFromA.velocity, pom);
		
		a->position = mat::add(a->position, mat::multiplyScalar(normal, -SMALL_AMOUNT));
		b->position = mat::add(b->position, mat::multiplyScalar(normal, SMALL_AMOUNT));
		int asdfasdf = 3;
		//a->velocity = aFromA.velocity;
	}
	
	/*
	nadji tacnu tacnu tacku gde se seku
	uzmes radijalni vektor do te tacke, e tu primeni silu
	*/
}
void SimulationManager::checkIdealGasLaw() {
	float avarageSpeed, avarageSquareSpeed;
	avarageSpeed = avarageSquareSpeed = 0;
	for (int i = 0; i < nrCollidables; i++) {
		avarageSpeed += mat::module(balls[i].velocity);
		avarageSquareSpeed += mat::module(balls[i].velocity) * mat::module(balls[i].velocity);
	}
	avarageSpeed /= nrCollidables;
	avarageSquareSpeed /= nrCollidables;
	std::cout << "checkIdealGasLaw(), avarageSpeed = " << avarageSpeed << " sqrt(avarageSquareSpeed) = " << sqrt(avarageSquareSpeed) << " relativeError = " << (1.0 - (sqrt(8.0 / 3.0 / 3.141592f) / (avarageSpeed / sqrt(avarageSquareSpeed)))) * 100.0 << "%" << std::endl;
}
void SimulationManager::doShit() {

	if (nrCollidables == 0)
		return;

	for(int i = 0; i < nrCollidables; i++)
		for (int j = i + 1; j < nrCollidables; j++){
			if (i != j) {
				collide(&balls[i], &balls[j]);
			}
		}

	float totalEnergy = 0;
	for (int i = 0; i < nrCollidables; i++) {
		totalEnergy += balls[i].getEnergy();
		//std::cout << "energy[" << i << "] = " << balls[i].getEnergy() << std::endl;
		if (balls[i].position.y < 0) {
			balls[i].velocity.y = -balls[i].velocity.y;
			balls[i].position.y = SMALL_AMOUNT;
		}
		if (balls[i].position.y > gPhysicsManager.mapSizeX) { ///mapa je visoka kolko je i siroka
			balls[i].velocity.y = -balls[i].velocity.y;
			balls[i].position.y = gPhysicsManager.mapSizeX - SMALL_AMOUNT;
		}

		if (balls[i].position.x > gPhysicsManager.mapSizeX / 2) {
			balls[i].velocity.x = -balls[i].velocity.x;
			balls[i].position.x = gPhysicsManager.mapSizeX / 2 - SMALL_AMOUNT;
		}
		if (balls[i].position.x < -gPhysicsManager.mapSizeX / 2) {
			balls[i].velocity.x = -balls[i].velocity.x;
			balls[i].position.x = -gPhysicsManager.mapSizeX / 2 + SMALL_AMOUNT;
		}
		
		if (balls[i].position.z > gPhysicsManager.mapSizeZ / 2) {
			balls[i].velocity.z = -balls[i].velocity.z;
			balls[i].position.z = gPhysicsManager.mapSizeZ / 2 - SMALL_AMOUNT;
		}
		if (balls[i].position.z < -gPhysicsManager.mapSizeZ / 2) {
			balls[i].velocity.z = -balls[i].velocity.z;
			balls[i].position.z = -gPhysicsManager.mapSizeZ / 2 + SMALL_AMOUNT;
		}
		int asdf = 1;

		balls[i].position = mat::add(balls[i].position, mat::multiplyScalar(balls[i].velocity, gPhysicsManager.deltat));
		balls[i].velocity = mat::add(balls[i].velocity, mat::multiplyScalar(balls[i].acceleration, gPhysicsManager.deltat));
	}
	if (gGameLoopCounter % 100 == 0) {
		std::cout << "totalEnergy = " << totalEnergy << std::endl;
		checkIdealGasLaw();
	}

}
float SimulationManager::Shape::getEnergy() {
	extern SimulationManager gSimulationManager;
	return mat::module(velocity) * mat::module(velocity) * mass / 2.f + mass * gSimulationManager.g * position.y;
}