#include"input.h"

const float sensitivity = 0.002f;

void Input::iOnUpdate()
{
    doPlayerControllerPolling();
}

bool Input::iIsKeyPressed(int keycode)
{
    unsigned char status = glfwGetKey(Application::getInstance()->getWindow()->pWindow, keycode);
    return status == GLFW_PRESS || status == GLFW_REPEAT;
}

bool Input::iIsMouseButtonPressed(int mousecode)
{
    return glfwGetMouseButton(Application::getInstance()->getWindow()->pWindow, mousecode) == GLFW_PRESS;
}

Vec2f Input::iGetMousePosition()
{
    double x, y;
    glfwGetCursorPos(Application::getInstance()->getWindow()->pWindow, &x, &y);

    return { (float)x, (float)y };
}

void Input::iUseRawMouseMotion() // Usefull for 3D fps camera controllers. Disabled mouse acceleration && scaling from the desktop
{                                           // Requirements: disabled cursor

    RV_ASSERT(glfwRawMouseMotionSupported(), "raw mouse motion not supported, maybe you didn't disable the cursor beforehand?");
    glfwSetInputMode(Application::getInstance()->getWindow()->pWindow, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
}

void Input::iDisableCursor()
{
    glfwSetInputMode(Application::getInstance()->getWindow()->pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Input::iHideCursor()
{
    glfwSetInputMode(Application::getInstance()->getWindow()->pWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

void Input::iUseNormalCursor()
{
    glfwSetInputMode(Application::getInstance()->getWindow()->pWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // Reverts the effects of GLFW_CURSOR_DISABLED && GLFW_CURSOR_HIDDEN
}

Vec2f Input::iGetCursorPosition()
{
    return m_MousePosition;
}

void Input::iOnEvent(Event* event)
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
    float sensitivity = 0.005;

    static Vec2f oldMousePosition = ((EventMouseMoved*)event)->m_Position;
    Vec2f deltaMouse = subtract(((EventMouseMoved*)event)->m_Position, oldMousePosition);
    oldMousePosition = ((EventMouseMoved*)event)->m_Position;


    Scene::getPlayerEntity()->get<TransformComponent>()->rotation.yaw -= deltaMouse.x * sensitivity;
    Scene::getPlayerEntity()->get<TransformComponent>()->rotation.pitch -= deltaMouse.y * sensitivity;

    if (Scene::getPlayerEntity()->get<TransformComponent>()->rotation.pitch >= degreesToRadians(90))
    {
        Scene::getPlayerEntity()->get<TransformComponent>()->rotation.pitch = degreesToRadians(90);
    }
    else if (Scene::getPlayerEntity()->get<TransformComponent>()->rotation.pitch <= degreesToRadians(-90))
    {
        Scene::getPlayerEntity()->get<TransformComponent>()->rotation.pitch = degreesToRadians(-90);
    }
}

void Input::doPlayerControllerPolling()
{
    auto* player = Scene::getPlayerEntity();
    
    float speed = 6;
    float verticalSpeed = 6;

    Vec3f* playerPos = &player->get<TransformComponent>()->position;
    Vec3f moveDirection(0, 0, 0);
    Vec3f aimDirection = getDirectionFromRotation(player->get<TransformComponent>()->rotation);

    if(isKeyPressed(RV_KEY_W))
        moveDirection = aimDirection;
    
    if(isKeyPressed(RV_KEY_S))
        moveDirection = -aimDirection;

    Vec3f rightDirection;
    rightDirection = cross(aimDirection, Vec3f(0, 0, 1));

    if(isKeyPressed(RV_KEY_D))
        moveDirection += rightDirection;

    if(isKeyPressed(RV_KEY_A))
        moveDirection += -rightDirection;

    Vec3f hDelta = normalise(moveDirection) * speed * Time::getDelta();

    if(isKeyPressed(RV_KEY_W) || isKeyPressed(RV_KEY_A) || isKeyPressed(RV_KEY_S) || isKeyPressed(RV_KEY_D))
    {
        *playerPos += hDelta;
    }

    Vec3f verticalDelta(0,0,0);
    if(isKeyPressed(RV_KEY_SPACE))
        verticalDelta = verticalSpeed * Vec3f(0, 0, 1) * Time::getDelta();

    if(isKeyPressed(RV_KEY_LEFT_SHIFT))
        verticalDelta = -verticalSpeed * Vec3f(0, 0, 1) * Time::getDelta();
    *playerPos += verticalDelta;
    
}
