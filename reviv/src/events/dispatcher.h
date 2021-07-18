#pragma once

#include"event.h"
#include"core/application.h"

class EventDispatcher
{
public:
    static void dispatch(Event* event)
    {
        switch(event->m_Type)
        {
            case(EventType::EventTypeWindowClose):
                Application::onEvent(event);
                break;
            case(EventType::EventTypeWindowResize):
                Application::onEvent(event);
                break;
            case(EventType::EventTypeMouseMoved):
                Input::onEvent(event);
                break;
            default:
                RV_ASSERT(false, "Event type not supported by the event dispatcher");
        }

    }
};