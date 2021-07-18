#pragma once

#include<iostream>

#include<GLFW/glfw3.h>

using std::cin; using std::cout; using std::endl;

class Time
{
public:
    static void onUpdate() { getInstance()->iOnUpdate(); }
    static float getTimeInSeconds() { return getInstance()->iGetTimeInSeconds(); }
    static float getDelta() { return getInstance()->iGetDelta(); } // returns time in dt in seconds
    static float isOneSecond() { return getInstance()->iIsOneSecond(); }

    static Time* getInstance()
    {
        static Time instance;
        return &instance;
    }

private:
    void iOnUpdate();
    float iGetDelta();
    float iGetTimeInSeconds();
    bool iIsOneSecond();

    bool m_OneSecond = false;
    float time;
    float delta;
};