#pragma once

#include"core/keycodes.h"
#include"core/mat.h"
#include"application.h"
#include"scene/scene.h"
#include"core/mat.h"
#include"scene/components.h"
#include"core/time.h"

class Input
{
public:
    static void onUpdate() { getInstance()->iOnUpdate(); }
    static bool isKeyPressed(int keycode) { return getInstance()->iIsKeyPressed(keycode); }
    static bool isMouseButtonPressed(int mousecode) { return getInstance()->iIsMouseButtonPressed(mousecode); }
    static Vec2f getMousePosition(){ return getInstance()->iGetMousePosition(); }

    static void useRawMouseMotion() { getInstance()->iUseRawMouseMotion(); }
    static void disableCursor() { getInstance()->iDisableCursor(); }
    static void hideCursor() { getInstance()->iHideCursor(); }
    static void useNormalCursor() { getInstance()->iUseNormalCursor(); }
    static Vec2f getCursorPosition() { return getInstance()->iGetCursorPosition(); }

    static void onEvent(Event* event) { getInstance()->iOnEvent(event); }

private:
    Input() = default;

    void iOnUpdate();

    bool iIsKeyPressed(int keycode);
    bool iIsMouseButtonPressed(int mousecode);
    Vec2f iGetMousePosition();
    void iUseRawMouseMotion();
    void iDisableCursor();
    void iHideCursor(); 
    void iUseNormalCursor();
    Vec2f iGetCursorPosition();

    void iOnEvent(Event* event);
    void onEventMouseMoved(Event* event);
    void doPlayerControllerOnEventMouseMoved(Event* event);
    void doPlayerControllerPolling();

    static Input* getInstance()
    {
        static Input instance;
        return &instance;
    }

    Vec2f m_MousePosition;

};