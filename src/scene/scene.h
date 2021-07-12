#pragma once
#include"stls/stable_vector.h"

#include<vector>

#include"entity.h"

class Scene{
public:
    Scene(const Scene&) = delete;
    ~Scene() = default;
    Scene& operator=(const Scene& other) = delete;

    static Entity* createEntity(const std::string& entityName = "NamelessEntity") { return getInstance().iCreateEntity(entityName); }
    static Entity* findEntity(const std::string& entityName) { return getInstance().iFindEntity(entityName); }
    static void logEntity(const std::string& entityName) { return getInstance().iLogEntity(entityName); }
    static stls::StableVector<Entity>* getEntityList() { return getInstance().iGetEntityList(); }

    static Entity* getCameraEntity() { return getInstance().iGetCameraEntity(); }
    static void setCameraEntity(Entity* pNewCameraEntity) {getInstance().iSetCameraEntity(pNewCameraEntity); }
    static Entity* getPlayerEntity() {cout << "RETARDDDDDD\n"; return nullptr; }
    static Entity* getPlayerEntity(int a) { cout << "REQUESTING " << a << endl; return getInstance().iGetPlayerEntity(); }
    static void setPlayerEntity(Entity* pNewPlayerEntity) { return getInstance().iSetPlayerEntity(pNewPlayerEntity); }
    ///
    stls::StableVector<Entity> entityList;

    Entity* pCameraEntity;
    Entity* pPlayerEntity;

    bool doesPlayerEntityExist = false;
    bool doesCameraEntityExist = false;

    static Scene& getInstance()
    {
        static Scene instance;
        return instance;
    }
private:
    Scene() : entityList(50) { }

    Entity* iCreateEntity(const std::string& entityName);
    Entity* iFindEntity(const std::string& entityName);
    void iLogEntity(const std::string& entityName);
    std::vector<Entity>* iGetEntityList() const;

    stls::StableVector<Entity>* iGetEntityList();

    Entity* iGetCameraEntity();
    void iSetCameraEntity(Entity* pNewCameraEntity);

    Entity* iGetPlayerEntity();
    void iSetPlayerEntity(Entity* pNewPlayerEntity);
};