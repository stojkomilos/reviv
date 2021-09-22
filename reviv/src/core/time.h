#pragma once

#include"rv_pch.hpp"

using std::cin; using std::cout; using std::endl;

class Time
{
public:

    static Time* get()
    {
        static Time instance;
        return &instance;
    }
    void init();
    void onUpdate();

    float getDelta(); // returns time in dt in seconds
    float getDeltaRealtime(); // returns real time (irl time) between this frame and the last
    float getTime(); // returns time since the start of rendering
    bool isOneSecond();

    float getLoopCounter() { return get()->gameLoopCounter; }

    float timeRatio = 1.f; // used for speeding up or slowing down game time

private:
    unsigned long long gameLoopCounter = 0;

    bool m_OneSecond = false;
    float time = 0.0f;
    float delta;

    float runLoopStartTime = 0.0f;
    bool isInited = false;
};