#include"file_manager.h"


//int FileManager::setUpWorld() {
//	FILE* world = fopen("save/world.ini", "r");   ///format world.ini glasi: prva tri broja su intigeri i oni su dimenzije sveta, cetvrti ukupan broj kocaka terena na mapi, svi ostali brojevi posle ta 3,(ima ih jos x * y * z) su vrednosti kocaka na mestu i,j,k(x,y,z nije u world space-u, prvo se konvertuje)
//	if (world != NULL) {
//		int x, y, z, n;
//		x = y = z = n = 0;
//		std::cout << "OPENING EXISTING WORLD OF SIZE " << std::endl;
//		bool supressWarning6031 = fscanf(world, "%d %d %d %d ", &x, &y, &z, &n); ////&x vs x
//		std::cout << x << " x " << y << " x " << z << std::endl << "numberOfTerrainCubes = " << n << std::endl;
//		gPhysicsManager.mapSizeX = x;
//		gPhysicsManager.mapSizeY = y;
//		gPhysicsManager.mapSizeZ = z;
//		gPhysicsManager.numberOfTerrainCubesOnStart = gPhysicsManager.numberOfTerrainCubesCurrently = n;
//		gPhysicsManager.worldIsLoadedFromFile = true;
//		gPhysicsManager.kocke = new Entity[gPhysicsManager.numberOfTerrainCubesCurrently + 10]; ///td: - 1
//		assert(gPhysicsManager.mapSizeX % 2 == 0 and gPhysicsManager.mapSizeZ % 2 == 0);
//		assert(gPhysicsManager.mapSizeX == gPhysicsManager.mapSizeZ);
//
//		gPhysicsManager.blockMap.setUp(x, y, z);
//		int counterForWorldLoading = 0;
//		for (int i = 0; i < x; i++) {
//			for (int j = 0; j < y; j++) {
//				for (int k = 0; k < z; k++) {
//					char value = 0; ///unisgnhed char
//					supressWarning6031 = fscanf(world, "%c ", &value);
//					value -= '0';
//
//					*gPhysicsManager.blockMap.adr(i, j, k) = value;
//					//std::cout << "ucitana vrednost = " << value << std::endl;
//					(gPhysicsManager.kocke + counterForWorldLoading)->position = Vec3f(i - gPhysicsManager.mapSizeX / 2 + 0.5f, j - 0.5f, k - gPhysicsManager.mapSizeZ / 2 + 0.5f);
//					(gPhysicsManager.kocke + counterForWorldLoading)->valid = true;
//					if(value > 0)
//						counterForWorldLoading++;
//				 }
//
//			}
//		}
//		
//		std::cout << "\nWORLD LOADED" << std::endl;
//		fclose(world);
//	}
//	else {
//		std::cout << "WORLD NOT LOADED FROM FILE" << std::endl;
//		gPhysicsManager.worldIsLoadedFromFile = false;
//	}
//	
//	return 0;
//} ///todo:sejv sveta posle stvaranja i proveri jel si to vec radio
//int FileManager::saveCurrentWorld() {
//
//	std::cout << "STARTING TO SAVE THE WORLD" << std::endl;
//
//	FILE* world = fopen("save/world.ini", "w"); 
//	
//	
//	fprintf(world, "%d %d %d %d", gPhysicsManager.mapSizeX, gPhysicsManager.mapSizeY, gPhysicsManager.mapSizeZ, gPhysicsManager.numberOfTerrainCubesCurrently);
//	fprintf(world, " ");
//
//
//
//	for (int i = 0; i < gPhysicsManager.mapSizeX; i++) {
//		for (int j = 0; j < gPhysicsManager.mapSizeY; j++) {
//			for (int k = 0; k < gPhysicsManager.mapSizeZ; k++) {
//				char c = (*gPhysicsManager.blockMap.adr(i, j, k)) + '0';
//				fprintf(world, "%c ", c);
//				
//			}
//		}
//	}
//	
//	fclose(world);
//	
//	std::cout << "COMPLETED WORLD SAVING" << std::endl;
//
//	return 0;
//	
//}
	
