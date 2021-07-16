#pragma once

#include"core/mat.h"
#include"application.h"

class Input
{
public:
    static bool isKeyPressed(int keycode) { return getInstance()->iIsKeyPressed(keycode); }
    static bool isMouseButtonPressed(int mousecode) { return getInstance()->iIsMouseButtonPressed(mousecode); }
    static Vec2f getMousePosition(){ return getInstance()->iGetMousePosition(); }

    static Input* getInstance()
    {
        static Input instance;
        return &instance;
    }

private:
    Input() = default;
    bool iIsKeyPressed(int keycode);
    bool iIsMouseButtonPressed(int mousecode);
    Vec2f iGetMousePosition();
};