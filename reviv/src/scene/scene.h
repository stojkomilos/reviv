#pragma once
#include"stls/stable_vector.h"

#include<vector>

#include"entity.h"
#include"components.h"

class Scene{
public:
    ~Scene() = default;
    Scene(const Scene&) = delete;
    Scene& operator=(const Scene& other) = delete;

    static Entity* createEntity(const std::string& entityName = "NamelessEntity") { return getInstance()->iCreateEntity(entityName); }
    static Entity* findEntity(const std::string& entityName) { return getInstance()->iFindEntity(entityName); }
    static void logEntity(const std::string& entityName) { return getInstance()->iLogEntity(entityName); }
    static stls::StableVector<Entity>* getEntityList() { return getInstance()->iGetEntityList(); }

    static Entity* getCameraEntity() { return getInstance()->iGetCameraEntity(); }
    static Entity* setCameraEntity(Entity* pNewCameraEntity) { return getInstance()->iSetCameraEntity(pNewCameraEntity); }
    static Entity* getPlayerEntity() { return getInstance()->iGetPlayerEntity(); }
    static Entity* setPlayerEntity(Entity* pNewPlayerEntity) { return getInstance()->iSetPlayerEntity(pNewPlayerEntity); }

    static Scene* getInstance()
    {
        static Scene instance;
        return &instance;
    }

    stls::StableVector<Entity> entityList;

    Entity* pCameraEntity;
    Entity* pPlayerEntity;

    bool doesPlayerEntityExist = false;
    bool doesCameraEntityExist = false;

private:
    Scene() : entityList(5000000) { }

    Entity* iCreateEntity(const std::string& entityName);
    Entity* iFindEntity(const std::string& entityName);
    void iLogEntity(const std::string& entityName);
    stls::StableVector<Entity>* iGetEntityList();

    Entity* iGetCameraEntity();
    Entity* iSetCameraEntity(Entity* pNewCameraEntity);

    Entity* iGetPlayerEntity();
    Entity* iSetPlayerEntity(Entity* pNewPlayerEntity);

    void projectPosition(const Entity& entity);
};