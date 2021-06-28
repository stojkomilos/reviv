#pragma once

#include"reviv/core/mat.h"
#include"reviv/physics/physics_manager.h"

class SimulationManager {
public:
	class Shape {
	public:
		Shape();
		mat::vec3 position, velocity, acceleration, force;
		float mass;
		mat::vec3 prevPosition;
		unsigned char type;
		float getEnergy();
	};
	class Ball : public Shape {
	public:
		Ball();
		float radius;
	};
	SimulationManager();
	void setUp();
	void doShit();
	void collide(Ball *a, Ball *b);
	void generateBall(Ball* a);
	void checkIdealGasLaw();
	Ball* balls = nullptr;
	int nrCollidables;
	float g;
	float totalEnergy;
	const float SMALL_AMOUNT = 0.001f;
};