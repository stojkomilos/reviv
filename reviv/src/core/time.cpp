#include<GLFW/glfw3.h>

#include"time.h"

void Time::iOnUpdate()
{
    float currentTime = glfwGetTime();
    delta = currentTime - time;
    time = currentTime;

    static float fpsTimer = currentTime;
    if(currentTime - fpsTimer > 1) // print the FPS every second
    {
        cout << "FPS: " << 1 / getDelta() << endl;
        fpsTimer = currentTime;
    }

}

float Time::iGetTimeInSeconds()
{
    return time;
}

float Time::iGetDelta()
{
    return delta;
}