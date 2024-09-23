#pragma once
#include "ECS/SystemManager.h"
#include <chrono>

namespace Carmicah
{
    class FPSCounter
    {
    private:
        std::chrono::steady_clock::time_point lastUpdateTime;
        double updateInterval;
        int frameCount;
        double currentFPS;

    public:
        FPSCounter() : updateInterval(0.5), frameCount(0), currentFPS(0.0) {}

        void Init();
        void Update();
        void Exit();

        double GetFPS() const { return currentFPS; }
    };
}