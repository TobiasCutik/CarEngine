#pragma once

namespace CE
{
    class Timer {

    public:
        Timer() = default;

        void start();
        void pause();
        void resume();
        void stop();

        float getTime();

    private:
        void updateTime();

        bool bStarted = false;
        bool bPaused = false;

        float currentTime = 0.f;
        float lastUpdatedTime = 0.f;
    };
}