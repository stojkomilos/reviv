#pragma once
#include"reviv/core/mat.h"
#include"reviv/entity/player.h"
#include"reviv/core/memory_manager.h"
#include"reviv/core/file_manager.h"
#include"reviv/core/random_manager.h"
#include"reviv/core/memory_manager.h"
#include<assert.h>
#include<cmath>
class PhysicsManager {
public:
    PhysicsManager();
    void startUp(int mapSizeX1, int mapSizeY1, int mapSizeZ1, float gravity);
    void shutDown();
    void doShit();
    void collidePlayerWithVoxelTerrain();
    float rayCastVoxelTerrain(mat::vec3 p0, mat::vec3 p1);
    bool checkIfOutOfMap(mat::vec3 a);
    void setPlayerHitbox(mat::vec3, bool newOrOld);
    unsigned char checkIfInsideVoxelTerrain(mat::vec3 position);
    unsigned char collideTwoPrisms(mat::vec3 aPosition, mat::vec3 aPreviousPosition, mat::vec3 bPosition, mat::vec3 bHitboxSize);
    void collideTwoPrismsHelper(float &dist1, float &dist2, mat::vec3 &aPreviousPosition, unsigned char &strana, unsigned char &prevStrana, float x, float y, float z);
    bool collidePlayerWithVoxelBlock(mat::vec3 blockPosition);
    struct hitbox {
        mat::vec3 vertex[8];
        void update(mat::vec3 position);
        void update(mat::vec3 position, mat::vec3(size));
    };
    struct TripletOfInts {
        int x, y, z;
    };


    hitbox playerHitbox;
    hitbox previousPlayerHitbox;
    int mapSizeX, mapSizeY, mapSizeZ;
    float spawnHeight, g;
    float deltat;
    bool worldIsLoadedFromFile;
    int numberOfTerrainCubesOnStart, numberOfTerrainCubesCurrently;
    Player player;
    Entity* kocke;
    TripletOfInts worldToArrayCords(mat::vec3 postion);
    MemoryManager::Array3D<char> blockMap;
    mat::vec3 arrayToWorldCords(int x, int y, int z);
    mat::vec3 surroundingCubes[150];
    int qRecentCollision[150], qRecentCollisionSize;
};