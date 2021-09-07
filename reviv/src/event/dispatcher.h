#pragma once

#include"event.h"
#include"core/application.h"
#include"renderer/render_manager.h"

class EventDispatcher
{
public:
    static void dispatch(Event* event)
    {
        switch(event->m_Type)
        {
            case(EventType::EventTypeWindowClose):
                Application::get()->onEvent(event);
                break;
            case(EventType::EventTypeWindowResize):
                Application::get()->onEvent(event);
                RenderManager::get()->onEvent(event);
                break;
            case(EventType::EventTypeMouseMoved):
                Input::get()->onEvent(event);
                break;
            default:
                RV_ASSERT(false, "Event type not supported by the event dispatcher");
        }

    }
};