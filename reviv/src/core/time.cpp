#include<GLFW/glfw3.h>

#include"time.h"

void Time::iOnUpdate()
{
    float currentTime = glfwGetTime();
    delta = currentTime - time;
    time = currentTime;
}

float Time::iGetTimeInSeconds()
{
    return time;
}

float Time::iGetDelta()
{
    return delta;
}