#pragma once
#include"stls/stable_vector.h"

#include<vector>

#include"entity.h"

class Scene{
public:
    ~Scene() = default;
    Scene(const Scene&) = delete;
    Scene& operator=(const Scene& other) = delete;

    static Entity* createEntity(const std::string& entityName = "NamelessEntity") { return get()->iCreateEntity(entityName); }
    static Entity* getEntity(const std::string& entityName) { return get()->iGetEntity(entityName); }
    static void removeEntity(Entity* pEntity) { get()->iRemoveEntity(pEntity); }
    static bool isEntityValid(Entity* pEntity) { return get()->iIsEntityValid(pEntity); }
    static void logEntity(const std::string& entityName) { return get()->iLogEntity(entityName); }
    static stls::StableVector<Entity>* getEntityList() { return get()->iGetEntityList(); }

    static Entity* getCameraEntity() { return get()->iGetCameraEntity(); }
    static Entity* setCameraEntity(Entity* pNewCameraEntity) { return get()->iSetCameraEntity(pNewCameraEntity); }
    static Entity* getPlayerEntity() { return get()->iGetPlayerEntity(); }
    static Entity* setPlayerEntity(Entity* pNewPlayerEntity) { return get()->iSetPlayerEntity(pNewPlayerEntity); }
    static float getGravity() { return get()->iGetGravity(); }

    static Scene* get()
    {
        static Scene instance;
        return &instance;
    }

    stls::StableVector<Entity> entityList;

    Entity* pCameraEntity;
    Entity* pPlayerEntity;

    bool doesPlayerEntityExist = false;
    bool doesCameraEntityExist = false;

    float gravity = 9.81f;

private:
    Scene() : entityList(5000000) { }

    Entity* iCreateEntity(const std::string& entityName);
    Entity* iGetEntity(const std::string& entityName);
    void iRemoveEntity(Entity* pEntity);
    bool iIsEntityValid(Entity* pEntity);
    void iLogEntity(const std::string& entityName);
    stls::StableVector<Entity>* iGetEntityList();

    Entity* iGetCameraEntity();
    Entity* iSetCameraEntity(Entity* pNewCameraEntity);

    Entity* iGetPlayerEntity();
    Entity* iSetPlayerEntity(Entity* pNewPlayerEntity);

    inline float iGetGravity() { return gravity; }

    void projectPosition(const Entity& entity); // for debug
};