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

    static Input* get()
    {
        static Input instance;
        return &instance;
    }

    void onEvent(Event* event);

    bool isKeyPressed(int keycode);
    bool isMouseButtonPressed(int mousecode);
    Vec2f getMousePosition();
    void useRawMouseMotion();
    void disableCursor();
    void hideCursor(); 
    void useNormalCursor();
    Vec2f getCursorPosition();

    void onEventMouseMoved(Event* event);
    void doPlayerControllerOnEventMouseMoved(Event* event);
    void doPlayerControllerPolling();

    void onUpdate();

private:
    Input() = default;

    Vec2f m_MousePosition;
};