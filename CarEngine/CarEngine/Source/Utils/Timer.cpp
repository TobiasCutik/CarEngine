#include "Timer.h"
#include <GLFW/glfw3.h>

namespace CE
{
    void Timer::start()
    {
        bStarted = true;
        bPaused = false;
        currentTime = 0.f;
        lastUpdatedTime = (float) glfwGetTime();
    }

    void Timer::pause()
    {
        updateTime();
        bPaused = true;
    }

    void Timer::resume()
    {
        bPaused = false;
        lastUpdatedTime = (float) glfwGetTime();
    }

    void Timer::stop()
    {
        bStarted = false;
        bPaused = false;
        currentTime = 0.f;
        lastUpdatedTime = 0.f;
    }

    float Timer::getTime()
    {
        if (!bStarted)
            return 0.f;

        if (!bPaused)
            updateTime();

        return currentTime;
    }

    void Timer::updateTime()
    {
        auto globalTime = (float) glfwGetTime();
        float deltaTime = globalTime - lastUpdatedTime;
        lastUpdatedTime = globalTime;

        currentTime += deltaTime;
    }
}