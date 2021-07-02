#include "physics_manager.h"

extern mat::Camera camera;
extern FileManager gFileManager;
extern RandomManager gRandomManager;

extern mat::vec3 gPlayerInitialPosition;
extern mat::vec3 gPointLightPosition;

extern int gGameLoopCounter;
float hitboxWidth = 0.6f;
float hitboxDepth = 0.6f;
float playerHeight = 1.8f;
float hitboxHeight = playerHeight;

void PhysicsManager::startUp(int mapSizeX1, int mapSizeY1, int mapSizeZ1, float gravity) {

	srand(0);

	//player.getComponent("Player")->speed = 25.0f;
	player.getComponent("Player")->speed = 25.0f;

	player.flyUpSpeed = player.speed;

	player.creativeMode = true;
	player.position = gPlayerInitialPosition; ///todo: premesti nesto od ovog u world.ini
	player.velocity = mat::vec3(0, 0, 0);
	player.acceleration = mat::vec3(0.f, 0, 0.f);
	player.pitch = 0;
	player.yaw = 0;
	player.standingOnSurface = 0;
}
void PhysicsManager::cummulatePosition(){
	player.position.x += deltat * player.velocity.x;
	player.position.y += deltat * player.velocity.y;
	player.position.z += deltat * player.velocity.z;

	gPointLightPosition = gPlayerInitialPosition;
	gPointLightPosition.x += 5 * sin(glfwGetTime() * 2.4);
	gPointLightPosition.y += 0 ;
	gPointLightPosition.z += 5 * cos(glfwGetTime() * 2.4);

}
void PhysicsManager::cummulateVelocity(){
	player.velocity.x += deltat * player.acceleration.x;
	player.velocity.y += deltat * player.acceleration.y;
	player.velocity.z += deltat * player.acceleration.z;

}
void PhysicsManager::doShit() {

	//assert(abs(playerHeight - player.height) <= 0.01f);///ukloni

	if (!player.creativeMode) {
		//std::cout << "Player is in creative mode\n";
		//if(!player.standingOnSurface)
		player.acceleration.y = -g;
	}
	
	if (!player.walking) {
		player.velocity.x = 0;
		player.velocity.z = 0;
	}
	if(player.creativeMode)
		if (!(player.holdingFlyDownButton or player.holdingFlyUpButton)) {
			player.velocity.y = 0;
		}
		
	

	player.standingOnSurface = false;

	cummulatePosition();
	cummulateVelocity();


	//if (gGameLoopCounter != 0)
	//	collidePlayerWithVoxelTerrain();
	player.previousPosition = player.position;
	previousPlayerHitbox.update(player.previousPosition);

 
	if (!player.creativeMode) {
		if (player.standingOnSurface and !player.walking) {
			if (abs(player.velocity.x) <= 0.2) {
				player.velocity.x = 0;
				player.acceleration.x = 0;
			}
			// else player.acceleration.x = mat::sgn(player.velocity.x) * -12.0f; ///
			else player.velocity.x = 0;
			if (abs(player.velocity.z) <= 0.2) {
				player.velocity.z = 0;
				player.acceleration.z = 0;
			}
			else player.velocity.z = 0;
		   // else player.acceleration.z = mat::sgn(player.velocity.z) * -12.0f;

		}
	}
	else player.acceleration.x = player.acceleration.z = 0;


	player.updateUnitDirectionalVectors();

	//if (player.holdingLeftClick) {
	//	rayCastVoxelTerrain(player.position, mat::add(player.position, mat::multiplyScalar(player.direction, -player.destroyRange)));
	//}
}
void PhysicsManager::collidePlayerWithVoxelTerrain() {


	playerHitbox.update(player.position);


	mat::vec3 currentNearCube;
	TripletOfInts tempTripletOfInts = worldToArrayCords(player.previousPosition);
	// if (gGameLoopCounter % 75 == 0) {
	//     std::cout << "trojke = " << tempTripletOfInts.x << " " << tempTripletOfInts.y << " " << tempTripletOfInts.z << std::endl;
	//     player.previousPosition.log("prosla lokacija = ");
	// }
	mat::vec3 playerHeadCubePositon = arrayToWorldCords(tempTripletOfInts.x, tempTripletOfInts.y, tempTripletOfInts.z);
	// if(gGameLoopCounter % 75 == 0)
	//     playerHeadCubePositon.log("playerHeadCubePositon = ");
	assert(playerHeight > 1.01f and playerHeight < 1.99f); ///nece raditi collision prvih najblizih



	int currentQSize = 0;
	for (int i = 0; i < qRecentCollisionSize; i++){
		currentNearCube = mat::add(playerHeadCubePositon, surroundingCubes[qRecentCollision[i]]);

		if (checkIfInsideVoxelTerrain(currentNearCube)) {
			if (collidePlayerWithVoxelBlock(currentNearCube)) {
				qRecentCollision[currentQSize++] = qRecentCollision[i];
			}
		}
		qRecentCollisionSize = currentQSize;
	}



	for (int j = 0; j < 125; j++) {

		currentNearCube = mat::add(playerHeadCubePositon, surroundingCubes[j]);
		//if(mat::getMannhattanDistance(currentNearCube, playerHeadCubePositon))
		//if (gGameLoopCounter % 75 == 0 and j == 2) {
		//	player.previousPosition.log("j = 2 prevPos=");
		//	currentNearCube.log("curentNearCube=");
		//}
		if (checkIfInsideVoxelTerrain(currentNearCube)) {
			if (collidePlayerWithVoxelBlock(currentNearCube)) {
				qRecentCollision[qRecentCollisionSize++] = j;
			}
		}
	}
}

bool PhysicsManager::collidePlayerWithVoxelBlock(mat::vec3 blockPosition) {
	bool collided = false;
	for (int i = 0; i < 8; i++) {
		playerHitbox.update(player.position);
		mat::vec3 dot;
		dot = playerHitbox.vertex[i];
		if (checkIfInsideVoxelTerrain(dot)) {

			
			float x11 = blockPosition.x + 0.5f;
			float x12 = blockPosition.x - 0.5f;
			float y11 = blockPosition.y + 0.5f;
			float y12 = blockPosition.y - 0.5f;
			float z11 = blockPosition.z + 0.5f;
			float z12 = blockPosition.z - 0.5f;

			
			previousPlayerHitbox.update(player.previousPosition);
			mat::vec3 tackaSad = dot;
			mat::vec3 tackaPre = previousPlayerHitbox.vertex[i];
			//at::vec3 tackaPre = player.previousPosition;
			//tackaSad.y -= player.height;
			//tackaPre.y -= player.height;
			unsigned char strana = collideTwoPrisms(tackaSad, tackaPre, blockPosition, mat::vec3(1.f, 1.f, 1.f));
			if(gGameLoopCounter % 75 == 0)
				std::cout << "Strana = " << (int)strana << std::endl;

			//player.position.log("prosli=");
			//player.previousPosition.log("trennutni=");
			const float SMALL_AMOUNT = 0.00087f;
			mat::vec3 newPosition = player.position;
			mat::vec3 newVelocity = player.velocity;
			if (strana == 1) {
				newPosition.x = x12 - SMALL_AMOUNT - hitboxWidth / 2.f;
				newVelocity.x = mat::clampMax(0, newVelocity.x);
			}
			else if (strana == 2) {
				newPosition.x = x11 + SMALL_AMOUNT + hitboxWidth / 2.f;
				newVelocity.x = mat::clampMin(0, newVelocity.x);
			}
			else if (strana == 3) {
				newPosition.y = y12 - SMALL_AMOUNT - hitboxHeight + playerHeight;
				newVelocity.y = mat::clampMax(0, newVelocity.y);
			}
			else if (strana == 4) {
				newPosition.y = y11 + SMALL_AMOUNT + playerHeight;
				newVelocity.y = mat::clampMin(0, newVelocity.y);
				player.standingOnSurface = true;
			}
			else if (strana == 5) {
				newPosition.z = z12 - SMALL_AMOUNT - hitboxDepth / 2.f;
				newVelocity.z = mat::clampMax(0, newVelocity.z);
			}
			else if (strana == 6) {
				newPosition.z = z11 + SMALL_AMOUNT + hitboxDepth / 2.f;
				newVelocity.z = mat::clampMin(0, newVelocity.z);
			}
			else {
			   // std::cout << "SUDAR DETEKTOVAN ALI NIJE PRONADJEN\n";
			}
			if (strana != 0 and mat::module(mat::subtract(player.previousPosition, newPosition)) < player.speed * deltat * 5) {

				player.position = newPosition;
				player.velocity = newVelocity;
				playerHitbox.update(player.position);
				collided = true;
			}
			else {
				continue;
				if (strana) {
					player.previousPosition.log("prsdfgsfdgosla");
					newPosition.log("nova");
				}
				std::cout << "cancer\n";
			}
				
			//assert(strana != 0);
			break;
		}
	}
	return collided;
}
unsigned char PhysicsManager::collideTwoPrisms(mat::vec3 aPosition, mat::vec3 aPreviousPosition, mat::vec3 bPosition, mat::vec3 bHitboxSize) {
	//gledamo iz perspektive kutije za pocetak
	PhysicsManager::hitbox bHitbox;
	aPosition = mat::subtract(aPosition, bPosition);
	aPreviousPosition = mat::subtract(aPreviousPosition, bPosition);
	bPosition = mat::vec3(0, 0, 0);
	bHitbox.update(bPosition, bHitboxSize);

	float x1, y1, z1;
	unsigned char strana = 0;
	float dist1, dist2; //dist1, dist2, i prevStrana se koriste za helper funkciju ove
	dist1 = dist2 = 0;
	unsigned char prevStrana = 0;
	y1 = aPreviousPosition.y + (aPosition.y - aPreviousPosition.y) / (aPosition.x - aPreviousPosition.x) * (-bHitboxSize.x / 2.f - aPreviousPosition.x);
	z1 = aPreviousPosition.z + (aPosition.z - aPreviousPosition.z) / (aPosition.x - aPreviousPosition.x) * (-bHitboxSize.x / 2.f - aPreviousPosition.x);
	if (-bHitboxSize.y / 2.f < y1 and y1 < bHitboxSize.y / 2.f)
		if (-bHitboxSize.z / 2.f < z1 and z1 < bHitboxSize.z / 2.f) {
			strana = 1; ///sece levu stranu kocke
			//std::cout << "1\n";
			collideTwoPrismsHelper(dist1, dist2, aPreviousPosition, strana, prevStrana, -bHitboxSize.x / 2.f, y1, z1);

			//std::cout << "y1 = " << y1 << " " << " z1 = " << z1 << std::endl;
		}


	y1 = aPreviousPosition.y + (aPosition.y - aPreviousPosition.y) / (aPosition.x - aPreviousPosition.x) * (bHitboxSize.x / 2.f - aPreviousPosition.x);
	z1 = aPreviousPosition.z + (aPosition.z - aPreviousPosition.z) / (aPosition.x - aPreviousPosition.x) * (bHitboxSize.x / 2.f - aPreviousPosition.x);
	if (-bHitboxSize.y / 2.f < y1 and y1 < bHitboxSize.y / 2.f)
		if (-bHitboxSize.z / 2.f < z1 and z1 < bHitboxSize.z / 2.f) {
			//std::cout << "2\n";
			strana = 2; ///sece desnu stranu kocke
			collideTwoPrismsHelper(dist1, dist2, aPreviousPosition, strana, prevStrana, bHitboxSize.x / 2.f, y1, z1);
		}






	x1 = aPreviousPosition.x + (aPosition.x - aPreviousPosition.x) / (aPosition.y - aPreviousPosition.y) * (-bHitboxSize.y / 2.f - aPreviousPosition.y);
	z1 = aPreviousPosition.z + (aPosition.z - aPreviousPosition.z) / (aPosition.y - aPreviousPosition.y) * (-bHitboxSize.y / 2.f - aPreviousPosition.y);
	if (-bHitboxSize.x / 2.f < x1 and x1 < bHitboxSize.x / 2.f)
		if (-bHitboxSize.z / 2.f < z1 and z1 < bHitboxSize.z / 2.f) {
			strana = 3; ///sece donju stranu kocke
			//std::cout << "3\n";
			collideTwoPrismsHelper(dist1, dist2, aPreviousPosition, strana, prevStrana, x1, -bHitboxSize.y / 2.f, z1);
		}



	//aPosition.log("prosli=");
	//aPreviousPosition.log("trennutni=");
	float a = bHitboxSize.y / 2.f;
	x1 = aPreviousPosition.x + (aPosition.x - aPreviousPosition.x) / (aPosition.y - aPreviousPosition.y) * (bHitboxSize.y / 2.f - aPreviousPosition.y);
	z1 = aPreviousPosition.z + (aPosition.z - aPreviousPosition.z) / (aPosition.y - aPreviousPosition.y) * (bHitboxSize.y / 2.f - aPreviousPosition.y);
	if (-a < x1 and x1 < a) {
		if (-bHitboxSize.z / 2.f < z1 and z1 < bHitboxSize.z / 2.f) {
			strana = 4; ///sece gornju stranu kocke
			//std::cout << "4\n";
			//std::cout << "cetr\n";
			//std::cout << "x1 = " << x1 << " " << " z1 = " << z1 << std::endl;
			//std::cout << "a=" << a << std::endl;
			collideTwoPrismsHelper(dist1, dist2, aPreviousPosition, strana, prevStrana, x1, bHitboxSize.y / 2.f, z1);
		}
	   // std::cout << "uso je, a = " << a << " x1 = " << x1 << std::endl;
	}
	//bHitboxSize.log("bsize = ");
	
	



	x1 = aPreviousPosition.x + (aPosition.x - aPreviousPosition.x) / (aPosition.z - aPreviousPosition.z) * (-bHitboxSize.z / 2.f - aPreviousPosition.z);
	y1 = aPreviousPosition.y + (aPosition.y - aPreviousPosition.y) / (aPosition.z - aPreviousPosition.z) * (-bHitboxSize.z / 2.f - aPreviousPosition.z);
	if (-bHitboxSize.x / 2.f < x1 and x1 < bHitboxSize.x / 2.f)
		if (-bHitboxSize.y / 2.f < y1 and y1 < bHitboxSize.y / 2.f) {
			strana = 5; ///sece zadnju stranu kocke
			//std::cout << "5\n";
			collideTwoPrismsHelper(dist1, dist2, aPreviousPosition, strana, prevStrana, x1, y1, -bHitboxSize.z / 2.f);
		   
		}

	x1 = aPreviousPosition.x + (aPosition.x - aPreviousPosition.x) / (aPosition.z - aPreviousPosition.z) * (bHitboxSize.z / 2.f - aPreviousPosition.z);
	y1 = aPreviousPosition.y + (aPosition.y - aPreviousPosition.y) / (aPosition.z - aPreviousPosition.z) * (bHitboxSize.z / 2.f - aPreviousPosition.z);
	if (-bHitboxSize.x / 2.f < x1 and x1 < bHitboxSize.x / 2.f)
		if (-bHitboxSize.y / 2.f < y1 and y1 < bHitboxSize.y / 2.f) {
			strana = 6; ///sece prednju stranu kocke
			//std::cout << "6\n";
			collideTwoPrismsHelper(dist1, dist2, aPreviousPosition, strana, prevStrana, x1, y1, bHitboxSize.z / 2.f);
			
		}
	//assert(0);
	return strana;

}
void PhysicsManager::collideTwoPrismsHelper(float &dist1, float &dist2, mat::vec3 &aPreviousPosition, unsigned char &strana, unsigned char &prevStrana, float x, float y, float z) { ///funkcija koja racuna udaljenost pocetne tacke i mesta gde se sece stranica, koristi se da bi se nasla ta najmanja udaljenost da bi znao koju stranu sece prvu, a koju drugu
	if (abs(dist1) == 0) { ///potencijalno pitaj samo if *dist1 != 0
		dist1 = mat::module(mat::subtract(aPreviousPosition, mat::vec3(x, y, z)));
		prevStrana = strana;
	}
	else {
		dist2 = mat::module(mat::subtract(aPreviousPosition, mat::vec3(x, y, z)));
		if (dist1 < dist2)
			strana = prevStrana;
	   // else std::cout << "cancer prevStrana = " << prevStrana << " strana = " << strana << std::endl;
	}
}
///proveri el valja posle svih ovhih menjanja svega
float PhysicsManager::rayCastVoxelTerrain(mat::vec3 p0, mat::vec3 p1) {     
	
	const int partitions = 10000; ///smanji
	mat::vec3 dt = mat::multiplyScalar(mat::subtract(p1, p0), 1.0f / partitions);
	int i = 0;
	//std::cout << std::endl << std::endl;
	while (i <= partitions){
		if (!checkIfOutOfMap(p0)) {
		
			int a = (int)(p0.x + mapSizeX / 2);
			int b = (int)(p0.y + 1);
			int c = (int)(p0.z + mapSizeZ / 2);
			//p0.log("p0");
			if (*blockMap.adr(a, b, c) == 1) {      
				*blockMap.adr(a, b, c) = false;
				numberOfTerrainCubesCurrently--;
				for (int i = 0; i < numberOfTerrainCubesOnStart; i++) {
					if (mat::module(mat::subtract((kocke + i)->position, mat::vec3(a - mapSizeX / 2 + 0.5f, b - 0.5f, c - mapSizeZ / 2 + 0.5f))) <= 0.01f) {
						(kocke + i)->valid = false;
						break;
					}
				}
				return mat::module(mat::multiplyScalar(dt, i));
			}
		}
		p0 = add(p0, dt);
		i++;
	}
	return 0; ///potencijalno lose, nz sta da radim sa ovim returnom
}
void PhysicsManager::hitbox::update(mat::vec3 position, mat::vec3 size) {
	
	vertex[0].x = position.x - size.x / 2.0f; ///todo: popraviti nakon pretvaranja kord. sistema u right side iz left size.
	vertex[0].y = position.y - size.y / 2.0f;
	vertex[0].z = position.z - size.z / 2.0f;
	
	vertex[1].x = position.x + size.x / 2.0f;
	vertex[1].y = position.y - size.y / 2.0f;
	vertex[1].z = position.z - size.z / 2.0f;

	vertex[2].x = position.x + size.x / 2.0f;
	vertex[2].y = position.y + size.y / 2.0f;
	vertex[2].z = position.z - size.z / 2.0f;

	vertex[3].x = position.x - size.x / 2.0f;
	vertex[3].y = position.y + size.y / 2.0f;
	vertex[3].z = position.z - size.z / 2.0f;
	

	vertex[4].x = position.x - size.x / 2.0f;
	vertex[4].y = position.y - size.y / 2.0f;
	vertex[4].z = position.z + size.z / 2.0f;

	vertex[5].x = position.x + size.x / 2.0f;
	vertex[5].y = position.y - size.y / 2.0f;
	vertex[5].z = position.z + size.z / 2.0f;

	vertex[6].x = position.x + size.x / 2.0f;
	vertex[6].y = position.y + size.y / 2.0f;
	vertex[6].z = position.z + size.z / 2.0f;

	vertex[7].x = position.x - size.x / 2.0f;
	vertex[7].y = position.y + size.y / 2.0f;
	vertex[7].z = position.z + size.z / 2.0f;
}
void PhysicsManager::hitbox::update(mat::vec3 position) {
	
	vertex[0].x = position.x - hitboxWidth / 2.0f; ///todo: popraviti nakon pretvaranja kord. sistema u right side iz left size
	vertex[0].y = position.y - playerHeight;
	vertex[0].z = position.z - hitboxDepth / 2.0f;
	
	vertex[1].x = position.x + hitboxWidth / 2.0f;
	vertex[1].y = position.y - playerHeight;
	vertex[1].z = position.z - hitboxDepth / 2.0f;

	vertex[2].x = position.x + hitboxWidth / 2.0f;
	vertex[2].y = position.y - playerHeight + hitboxHeight;
	vertex[2].z = position.z - hitboxDepth / 2.0f;

	vertex[3].x = position.x - hitboxWidth / 2.0f;
	vertex[3].y = position.y - playerHeight + hitboxHeight;
	vertex[3].z = position.z - hitboxDepth / 2.0f;
	

	vertex[4].x = position.x - hitboxWidth / 2.0f;
	vertex[4].y = position.y - playerHeight;
	vertex[4].z = position.z + hitboxDepth / 2.0f;

	vertex[5].x = position.x + hitboxWidth / 2.0f;
	vertex[5].y = position.y - playerHeight;
	vertex[5].z = position.z + hitboxDepth / 2.0f;

	vertex[6].x = position.x + hitboxWidth / 2.0f;
	vertex[6].y = position.y - playerHeight + hitboxHeight;
	vertex[6].z = position.z + hitboxDepth / 2.0f;

	vertex[7].x = position.x - hitboxWidth / 2.0f;
	vertex[7].y = position.y - playerHeight + hitboxHeight;
	vertex[7].z = position.z + hitboxDepth / 2.0f;
}
bool PhysicsManager::checkIfOutOfMap(mat::vec3 a) {
	if (a.x > -mapSizeX / 2 and a.y > -1 and a.z > -mapSizeZ / 2 and a.x < mapSizeX / 2 and a.y < mapSizeY - 1 and a.z < mapSizeZ / 2)
		return false;
	else return true;
}
PhysicsManager::TripletOfInts PhysicsManager::worldToArrayCords(mat::vec3 position) {
	
	return { (int)(position.x + mapSizeX / 2.f), (int)(position.y + 1), (int)(position.z + mapSizeZ / 2.f) };
}
mat::vec3 PhysicsManager::arrayToWorldCords(int x, int y, int z) {
	return mat::vec3(x - mapSizeX / 2.f + 0.5f, y - 0.5f, z - mapSizeZ / 2.f + 0.5f);
}
unsigned char PhysicsManager::checkIfInsideVoxelTerrain(mat::vec3 position) {
	if (!checkIfOutOfMap(position)) {
		TripletOfInts a = worldToArrayCords(position);
		return *blockMap.adr(a.x, a.y, a.z);
	}
	return 0;
}
PhysicsManager::PhysicsManager() {
	mapSizeX = mapSizeY = mapSizeZ = 0;
	spawnHeight = g = deltat = 0;
	worldIsLoadedFromFile = numberOfTerrainCubesOnStart = numberOfTerrainCubesCurrently = 0;
	kocke = nullptr;
	qRecentCollisionSize = 0;
	qRecentCollision[0] = 0;
}