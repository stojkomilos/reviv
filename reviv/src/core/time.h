#pragma once

#include"rv_pch.hpp"

using std::cin; using std::cout; using std::endl;

class Time
{
public:
    static void init() { get()->iInit(); }
    static void onUpdate() { get()->iOnUpdate(); }
    static float getTime() { return get()->igetTime(); } // returns time since the start of rendering
    static float getDelta() { return get()->iGetDelta(); } // returns time in dt in seconds
    static float isOneSecond() { return get()->iIsOneSecond(); }
    static float getLoopCounter() { return get()->gameLoopCounter; }

    static Time* get()
    {
        static Time instance;
        return &instance;
    }

private:
    void iInit();
    void iOnUpdate();
    float iGetDelta();
    float igetTime();
    bool iIsOneSecond();
    unsigned long long gameLoopCounter = 0;

    bool m_OneSecond = false;
    float time = 0.0f;
    float delta;

    float runLoopStartTime = 0.0f;
    bool isInited = false;
};