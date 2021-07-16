#pragma once

class Time
{
public:
    static void onUpdate() { getInstance()->iOnUpdate(); }
    static float getTimeInSeconds() { return getInstance()->iGetTimeInSeconds(); }
    static float getDelta() { return getInstance()->iGetDelta(); } // returns time in dt in seconds

    static Time* getInstance()
    {
        static Time instance;
        return &instance;
    }

private:
    void iOnUpdate();
    float iGetDelta();
    float iGetTimeInSeconds();

    float time;
    float delta;
};