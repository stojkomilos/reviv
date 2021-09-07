#include"time.h"

#include"scene/scene.h"
#include"scene/components.h"

#include<GLFW/glfw3.h>

void Time::iInit()
{
    RV_ASSERT(isInited == false, "already initialized");
    isInited = true;

    runLoopStartTime = glfwGetTime();
}

void Time::iOnUpdate()
{
    RV_ASSERT(isInited == true, "not initialized");

    gameLoopCounter++;

    float currentTime = glfwGetTime() - runLoopStartTime;
    delta = currentTime - time;
    time = currentTime;

    static float fpsTimer = currentTime;
    if(currentTime - fpsTimer > 1)
    {
        fpsTimer = currentTime;
        m_OneSecond = true;
    }
    else m_OneSecond = false;

    if(Time::isOneSecond())
    {
        cout << "FPS: " << 1 / Time::getDelta() << endl;
        cout << "Position: ";
        log(Scene::getPlayerEntity()->get<TransformComponent>()->position);
        cout << "Rotation: ";
        log(Scene::getPlayerEntity()->get<TransformComponent>()->rotation);
    }

}

bool Time::iIsOneSecond()
{
    RV_ASSERT(isInited == true, "not initialized");

    return m_OneSecond;
}

float Time::igetTime()
{
    RV_ASSERT(isInited == true, "not initialized");

    return time;
}

float Time::iGetDelta()
{
    RV_ASSERT(isInited == true, "not initialized");

    return delta;
}