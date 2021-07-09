#pragma once

#include<vector>

#include"entity.h"

class Scene{ //TODO: delete assignment operator (operator=)
public:
    Scene(const Scene&) = delete;
    static Entity* createEntity(const std::string& entityName = "NamelessEntity") { return getInstance().iCreateEntity(entityName); }
    static Entity* findEntity(const std::string& entityName) { return getInstance().iFindEntity(entityName); }
    static void logEntity(const std::string& entityName) { return getInstance().iLogEntity(entityName); }
    static std::vector<Entity>* getEntityList() { return getInstance().iGetEntityList(); }

    static Entity* getCameraEntity() { return getInstance().iGetCameraEntity(); }
    static void setCameraEntity(Entity* pNewCameraEntity) {getInstance().iSetCameraEntity(pNewCameraEntity); }
    static Entity* getPlayerEntity() {cout << "RETARDDDDDD\n"; return nullptr; }
    static Entity* getPlayerEntity(int a) { cout << "REQUESTING " << a << endl; return getInstance().iGetPlayerEntity(); }
    static void setPlayerEntity(Entity* pNewPlayerEntity) { return getInstance().iSetPlayerEntity(pNewPlayerEntity); }
    ///
    std::vector<Entity> entityList;

    Entity* pCameraEntity;
    Entity* pPlayerEntity;

    bool doesPlayerEntityExist = false;
    bool doesCameraEntityExist = false;

    static Scene& getInstance() //nzm dal moze u private
    {
        static Scene instance;
        return instance;
    }
private:
    Scene() = default;

    Entity* iCreateEntity(const std::string& entityName);
    Entity* iFindEntity(const std::string& entityName);
    void iLogEntity(const std::string& entityName);
    std::vector<Entity>* iGetEntityList() const;
    std::vector<Entity>* iGetEntityList();

    Entity* iGetCameraEntity();
    void iSetCameraEntity(Entity* pNewCameraEntity);

    Entity* iGetPlayerEntity();
    void iSetPlayerEntity(Entity* pNewPlayerEntity);
};