#include "pch.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "CarmicahTime.h"

namespace Carmicah
{
    namespace CarmicahTimer
    {
        CarmicahTime timerObj;

        void StartTime()
        {
            timerObj.InitTime();
        }

        void UpdateElapsedTime()
        {
            timerObj.UpdateTime();
        }

        double GetDt()
        {
            return timerObj.GetDeltaTime();
        }

        double GetFPS()
        {
            return timerObj.FPS();
        }

        void StartSystemTimer(const std::string& systemName)
        {
            timerObj.StartSystemTimer(systemName);
        }

        void StopSystemTimer(const std::string& systemName)
        {
            timerObj.StopSystemTimer(systemName);
        }

        void StartLoopTimer()
        {
            timerObj.StartLoopTimer();
        }

        void StopLoopTimer()
        {
            timerObj.StopLoopTimer();
        }

        void CalculateSystemPercentages()
        {
            timerObj.CalculateSystemPercentages();
        }

        const std::unordered_map<std::string, double>& GetSystemPercentages()
        {
            return timerObj.GetSystemPercentages();
        }

        double GetTotalLoopTime()
        {
            return timerObj.GetTotalLoopTime();
        }

        void InitGPUProfiling()
        {
            timerObj.InitGPUProfiling();
        }

        void StartGPUTimer()
        {
            timerObj.StartGPUTimer();
        }

        void StopGPUTimer()
        {
            timerObj.StopGPUTimer();
        }

        double GetGPUTime()
        {
            return timerObj.GetGPUTime();
        }
    }
}