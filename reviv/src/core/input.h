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
    static void onUpdate() { get()->iOnUpdate(); }
    static bool isKeyPressed(int keycode) { return get()->iIsKeyPressed(keycode); }
    static bool isMouseButtonPressed(int mousecode) { return get()->iIsMouseButtonPressed(mousecode); }
    static Vec2f getMousePosition(){ return get()->iGetMousePosition(); }

    static void useRawMouseMotion() { get()->iUseRawMouseMotion(); }
    static void disableCursor() { get()->iDisableCursor(); }
    static void hideCursor() { get()->iHideCursor(); }
    static void useNormalCursor() { get()->iUseNormalCursor(); }
    static Vec2f getCursorPosition() { return get()->iGetCursorPosition(); }

    static void onEvent(Event* event) { get()->iOnEvent(event); }

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

    static Input* get()
    {
        static Input instance;
        return &instance;
    }

    Vec2f m_MousePosition;

};