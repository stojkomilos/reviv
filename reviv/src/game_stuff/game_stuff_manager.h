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

    Weather weather;

    void init();
    void onUpdate(float dt);
};