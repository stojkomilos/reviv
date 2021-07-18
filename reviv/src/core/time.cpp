#include"time.h"

void Time::iOnUpdate()
{
    float currentTime = glfwGetTime();
    delta = currentTime - time;
    time = currentTime;

    static float fpsTimer = currentTime;
    if(currentTime - fpsTimer > 1)
    {
        fpsTimer = currentTime;
        m_OneSecond = true;
    }
    else m_OneSecond = false;

}

bool Time::iIsOneSecond()
{
    return m_OneSecond;
}

float Time::iGetTimeInSeconds()
{
    return time;
}

float Time::iGetDelta()
{
    return delta;
}