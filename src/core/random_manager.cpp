#include"random_manager.h"

/*
extern PhysicsManager gPhysicsManager;

RandomManager::RandomManager() {
	sizeOfPerlinMap = 0;	
}

void RandomManager::setUpPerlin(int perlinMapSize) {

	std::cout << "Starting setting up perlin noise\n";
	sizeOfPerlinMap = perlinMapSize;
	
	gridGradient.setUp(sizeOfPerlinMap, sizeOfPerlinMap);
	gridPerlin.setUp(sizeOfPerlinMap, sizeOfPerlinMap);
	
	for (int i = 0; i < sizeOfPerlinMap; i++) {
		for (int j = 0; j < sizeOfPerlinMap; j++) {
			assert(i < 10000 and j < 10000);///perlin map size trenutno ide do 10000, ali se moze povecati
			float angle = rand() / (RAND_MAX * 0.5f) * 2.f * 3.14f;
			gridGradient.adr(i, j)->x = cos(angle);
			gridGradient.adr(i, j)->y = sin(angle);
		}
	}

	for (int i = 0; i < sizeOfPerlinMap - 1; i++) {
		for (int j = 0; j < sizeOfPerlinMap - 1; j++) {
			*gridPerlin.adr(i, j) = mat::dot(Vec2f(1, 0), Vec2f(gridGradient.adr(i + 1, j)->x, gridGradient.adr(i + 1, j)->y));
			*gridPerlin.adr(i, j) += mat::dot(Vec2f(0, 1), Vec2f(gridGradient.adr(i, j + 1)->x, gridGradient.adr(i, j + 1)->y));
			*gridPerlin.adr(i, j) += mat::dot(mat::normalise(Vec2f(1, 1)), Vec2f(gridGradient.adr(i + 1, j + 1)->x, gridGradient.adr(i + 1, j + 1)->y));
			*gridPerlin.adr(i, j) /= 3;
		}
	}

	std::cout << "Completed setting up perlin noise\n"; 
}
float RandomManager::lerp(float a, float b, float weightA) {
	return a * weightA + b * (1 - weightA);
}
float RandomManager::perlin(Vec2f position) {

	if (position.x - 1.01f > sizeOfPerlinMap or position.y - 1.01f > sizeOfPerlinMap) {
		std::cout << "ERROR: WORLD TRIED TO GENERATE OUTSIDE OF THE PERLIN MAP ";
		position.log(" ");
		return 0;
	}

	int x = (int)position.x;
	int y = (int)position.y;
	
	float p1 = *gridPerlin.adr(x, y);
	float p2 = *gridPerlin.adr(x + 1, y);
	float r1 = lerp(p1, p2, 1.f - position.x + x);

	p1 = *gridPerlin.adr(x, y + 1);
	p2 = *gridPerlin.adr(x + 1, y + 1);
	float r2 = lerp(p1, p2, 1.f - position.x + x);

	float result = lerp(r1, r2, 1.f - position.y + y);
	return result;
}

*/