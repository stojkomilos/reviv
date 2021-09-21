#include"input.h"

static const float sensitivity = 0.0035f;
#define RV_EPSILON 0.00001f

void Input::onUpdate()
{
    doPlayerControllerPolling();
}

bool Input::isKeyPressed(int keycode)
{
    unsigned char status = glfwGetKey(Application::get()->getWindow()->pWindow, keycode);
    return status == GLFW_PRESS || status == GLFW_REPEAT;
}

bool Input::isMouseButtonPressed(int mousecode)
{
    return glfwGetMouseButton(Application::get()->getWindow()->pWindow, mousecode) == GLFW_PRESS;
}

Vec2 Input::getMousePosition()
{
    double x, y;
    glfwGetCursorPos(Application::get()->getWindow()->pWindow, &x, &y);

    return { (float)x, (float)y };
}

void Input::useRawMouseMotion() // Usefull for 3D fps camera controllers. Disabled mouse acceleration && scaling from the desktop
{                                           // Requirements: disabled cursor

    RV_ASSERT(glfwRawMouseMotionSupported(), "raw mouse motion not supported, maybe you didn't disable the cursor beforehand?");
    glfwSetInputMode(Application::get()->getWindow()->pWindow, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
}

void Input::disableCursor()
{
    glfwSetInputMode(Application::get()->getWindow()->pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Input::hideCursor()
{
    glfwSetInputMode(Application::get()->getWindow()->pWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

void Input::useNormalCursor()
{
    glfwSetInputMode(Application::get()->getWindow()->pWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // Reverts the effects of GLFW_CURSOR_DISABLED && GLFW_CURSOR_HIDDEN
}

Vec2 Input::getCursorPosition()
{
    return m_MousePosition;
}

void Input::onEvent(Event* event)
{
    switch(event->m_Type)
    {
        case(EventType::EventTypeMouseMoved):
            onEventMouseMoved(event);
            break;
        default:
            RV_ASSERT(false, "event type not recognized");
    }
}

void Input::onEventMouseMoved(Event* event)
{
    EventMouseMoved* eventMouseMoved = (EventMouseMoved*)event;
    m_MousePosition = eventMouseMoved->m_Position;
   doPlayerControllerOnEventMouseMoved(event);
}

void Input::doPlayerControllerOnEventMouseMoved(Event* event)
{

    static Vec2 oldMousePosition = ((EventMouseMoved*)event)->m_Position;
    Vec2 deltaMouse = subtract(((EventMouseMoved*)event)->m_Position, oldMousePosition);
    oldMousePosition = ((EventMouseMoved*)event)->m_Position;


    *Scene::getPlayerEntity()->get<TransformComponent>()->getRotationPtr()->getPtr(2, 0) -= deltaMouse.get(0, 0) * sensitivity;
    *Scene::getPlayerEntity()->get<TransformComponent>()->getRotationPtr()->getPtr(1, 0) -= deltaMouse.get(1, 0) * sensitivity;

    if (Scene::getPlayerEntity()->get<TransformComponent>()->getRotation().get(1, 0) >= degreesToRadians(90))
    {
        *Scene::getPlayerEntity()->get<TransformComponent>()->getRotationPtr()->getPtr(1, 0) = degreesToRadians(90);
    }
    else if (Scene::getPlayerEntity()->get<TransformComponent>()->getRotation().get(1, 0) <= degreesToRadians(-90))
    {
        *Scene::getPlayerEntity()->get<TransformComponent>()->getRotationPtr()->getPtr(1, 0) = degreesToRadians(-90);
    }
}

void Input::doPlayerControllerPolling()
{
    auto* player = Scene::getPlayerEntity();
    
    float speed = 6;
    float verticalSpeed = speed;

    Vec3* playerPos = player->get<TransformComponent>()->getPositionPtr();
    Vec3 moveDirection(0, 0, 0);
    Vec3 aimDirection = getDirectionFromRotation(player->get<TransformComponent>()->getRotation());
    Vec3 rightDirection = cross(aimDirection, Vec3(0, 0, 1));

    bool pressedW = isKeyPressed(RV_KEY_W);
    bool pressedS = isKeyPressed(RV_KEY_S);
    bool pressedA = isKeyPressed(RV_KEY_A);
    bool pressedD = isKeyPressed(RV_KEY_D);

    if(pressedW && !pressedS)
        moveDirection += aimDirection;
    else if(!pressedW && pressedS)
        moveDirection -= aimDirection;

    if(pressedD && !pressedA)
        moveDirection += rightDirection;
    else if(!pressedD && pressedA)
        moveDirection -= rightDirection;


    if((pressedW != pressedS) || (pressedA != pressedD))
    {
        Vec3 horizontalPlaneDelta = moveDirection / (module(moveDirection) + RV_EPSILON) * speed * Time::get()->getDelta(); // TODO: the + RV_EPSILON thing is probably responsible for a movement bug
        *playerPos += horizontalPlaneDelta;
    }

    Vec3 verticalDelta(0,0,0);
    bool pressedSpace = isKeyPressed(RV_KEY_SPACE);
    bool pressedLeftShift = isKeyPressed(RV_KEY_LEFT_SHIFT);

    if(pressedSpace && !pressedLeftShift)
        verticalDelta = verticalSpeed * Vec3(0, 0, 1) * Time::get()->getDelta();
        else if(!pressedSpace && pressedLeftShift)
            verticalDelta = -verticalSpeed * Vec3(0, 0, 1) * Time::get()->getDelta();

    *playerPos += verticalDelta;
    
}
