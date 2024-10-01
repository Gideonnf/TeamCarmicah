// CarmicahTime.h

#ifndef CARMICAH_TIME_H
#define CARMICAH_TIME_H
#include "Singleton.h"
#include <GLFW/glfw3.h>
#include <Windows.h>
#include <chrono>
#include <unordered_map>
#include <string>

namespace Carmicah
{
    class CarmicahTime
    {
    private:
        std::chrono::steady_clock::time_point lastUpdateTime;

        double mUpdateTimer;
        double mUpdateInterval;
        int mFrameCount;
        double mCurrentFPS;
        double mDeltaTime;
        double mPrevTime;

        std::unordered_map<std::string, double> mSystemProfilingData;

    public:
        CarmicahTime() : mDeltaTime(0), mUpdateTimer(0.0), mUpdateInterval(0.5), mFrameCount(0), mCurrentFPS(0.0) {}
        ~CarmicahTime() {}

        void InitTime();
        void UpdateTime();
        void StartProfileTimer(const std::string& systemName);
        void EndProfileTimer(const std::string& systemName);
        const std::unordered_map<std::string, double>& GetProfilingData() const { return mSystemProfilingData; }

        inline double FPS() const { return mCurrentFPS; }
        inline double GetDeltaTime() const { return mDeltaTime; }
    };

    namespace CarmicahTimer
    {
        static CarmicahTime timerObj;

        void StartTime();
        void UpdateElapsedTime();
        double GetDt();
        double GetFPS();
        void StartProfileTimer(const std::string& systemName);
        void EndProfileTimer(const std::string& systemName);
        const std::unordered_map<std::string, double>& GetProfilingData();
    }
}

#endif