#pragma once

#include"weather.h"

class GameStuffManager
{
public:

    static GameStuffManager* get()
    {
        static GameStuffManager instance;
        return &instance;
    }

    static void init() { get()->iInit(); }
    static void onUpdate() { get()->iOnUpdate(); }

    Weather weather;

private:
    void iInit();
    void iOnUpdate();
};