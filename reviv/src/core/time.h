#pragma once

#include"rv_pch.hpp"

#include<GLFW/glfw3.h>

using std::cin; using std::cout; using std::endl;

class Time
{
public:
    static void onUpdate() { getInstance()->iOnUpdate(); }
    static float getTime() { return getInstance()->igetTime(); }
    static float getDelta() { return getInstance()->iGetDelta(); } // returns time in dt in seconds
    static float isOneSecond() { return getInstance()->iIsOneSecond(); }
    static float getLoopCounter() { return getInstance()->gameLoopCounter; }

    static Time* getInstance()
    {
        static Time instance;
        return &instance;
    }

private:
    void iOnUpdate();
    float iGetDelta();
    float igetTime();
    bool iIsOneSecond();
    unsigned long long gameLoopCounter = 0;

    bool m_OneSecond = false;
    float time;
    float delta;
};