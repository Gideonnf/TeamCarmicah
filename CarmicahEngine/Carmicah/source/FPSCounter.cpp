#include "pch.h"
#include "FPSCounter.h"
#include "ECS/SystemManager.h"
#include "ECS/ComponentManager.h"
#include <spdlog/spdlog.h>

namespace Carmicah
{
    void FPSCounter::Init()
    {
        // Initialize the last update time
        lastUpdateTime = std::chrono::steady_clock::now();
        frameCount = 0;
        currentFPS = 0.0;
    }

    void FPSCounter::Update()
    {
        frameCount++;

        auto currentTime = std::chrono::steady_clock::now();
        double elapsed = std::chrono::duration<double>(currentTime - lastUpdateTime).count();

        if (elapsed >= updateInterval)
        {
            currentFPS = static_cast<double>(frameCount) / elapsed;
            frameCount = 0;
            lastUpdateTime = currentTime;

            // Log the FPS using spdlog
            auto logger = spdlog::get("CARMICAH");
            if (logger)
            {
                logger->info("Current FPS: {:.2f}", currentFPS);
            }
        }
    }

    void FPSCounter::Exit()
    {
        // Clean up if necessary
    }
}