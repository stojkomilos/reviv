#pragma once

#include"scene/scene.h"

class Weather
{
public:
    Weather() = default;
    void init(const std::string& baseNameEntities, float timeInHours);

    void onUpdate();
    void setSunTimeOfDay(float timeInHours);

private:
    bool isInited = false;

    void setSunModelPosition();
    void setSunDirectionalLight();

    float m_TotalTimeInHours;
    std::string m_baseNameEntities;

    Vec3 sunDirection;
    Entity* pSunModel;
    Entity* pSunDirectionalLight;
};