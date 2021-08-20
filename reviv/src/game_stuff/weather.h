#pragma once

#include"scene/scene.h"

class Weather
{
public:
    void init(int timeOfDayHours, int timeOfDayMinutes, float timeOfDaySeconds);
    void setSunPosition(int timeOfDayHours, int timeOfDayMinutes, float timeOfDaySeconds);
    void setSunModelPosition();
    void setSunDirectionalLightPosition();
    void onUpdate();

    int m_TimeOfDayHours;
    int m_TimeOfDayMinutes;
    float m_TimeOfDaySeconds;

    Vec3f sunDirection;
    Entity* pSunModel;
    Entity* pSunDirectionalLight;
};