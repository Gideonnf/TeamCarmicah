#include "pch.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "CarmicahTime.h"

namespace Carmicah
{
    CarmicahTime* CarmicahTime::instance = nullptr;

    CarmicahTime* CarmicahTime::GetInstance()
    {
        if (instance == nullptr)
        {
            instance = new CarmicahTime();
        }
        return instance;
    }

    CarmicahTime::CarmicahTime()
        : mUpdateTimer(0.0), mUpdateInterval(0.5), mFrameCount(0),
        mCurrentFPS(0.0), mDeltaTime(0.0), mPrevTime(0.0),
        mTotalLoopTime(0.0), mGPUTime(0)
    {
        // Initialize other members as needed
    }

    CarmicahTime::~CarmicahTime()
    {
        // Cleanup if necessary
    }

    void CarmicahTime::InitTime()
    {
        lastUpdateTime = std::chrono::steady_clock::now();
        mPrevTime = glfwGetTime();
    }

    void CarmicahTime::UpdateTime()
    {
        mFrameCount++;

        double currTime = glfwGetTime();
        mDeltaTime = currTime - mPrevTime;
        mPrevTime = currTime;
        mUpdateTimer += mDeltaTime;

        if (mUpdateTimer >= mUpdateInterval)
        {
            mCurrentFPS = static_cast<double>(mFrameCount) / mUpdateTimer;
            mFrameCount = 0;
            mUpdateTimer = 0.0;
        }
    }

    void CarmicahTime::StartSystemTimer(const std::string& systemName)
    {
        mSystemStartTimes[systemName] = std::chrono::steady_clock::now();
    }

    void CarmicahTime::StopSystemTimer(const std::string& systemName)
    {
        auto endTime = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - mSystemStartTimes[systemName]);
        mSystemTimes[systemName] = duration.count() / 1000000.0; // Convert to seconds
    }

    void CarmicahTime::StartLoopTimer()
    {
        mLoopStartTime = std::chrono::steady_clock::now();
    }

    void CarmicahTime::StopLoopTimer()
    {
        auto endTime = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - mLoopStartTime);
        mTotalLoopTime = duration.count() / 1000000.0; // Convert to seconds
    }

    void CarmicahTime::CalculateSystemPercentages()
    {
        for (const auto& pair : mSystemTimes)
        {
            mSystemPercentages[pair.first] = (pair.second / mTotalLoopTime) * 100.0;
        }
    }

    void CarmicahTime::InitGPUProfiling()
    {
        glGenQueries(1, &mGPUQueryStart);
        glGenQueries(1, &mGPUQueryEnd);
    }

    void CarmicahTime::StartGPUTimer()
    {
        glQueryCounter(mGPUQueryStart, GL_TIMESTAMP);
    }

    void CarmicahTime::StopGPUTimer()
    {
        glQueryCounter(mGPUQueryEnd, GL_TIMESTAMP);

        // Wait until the results are available
        GLint stopTimerAvailable = 0;
        while (!stopTimerAvailable) {
            glGetQueryObjectiv(mGPUQueryEnd, GL_QUERY_RESULT_AVAILABLE, &stopTimerAvailable);
        }

        // Get query results
        glGetQueryObjectui64v(mGPUQueryStart, GL_QUERY_RESULT, &mGPUTime);
        GLuint64 endTime;
        glGetQueryObjectui64v(mGPUQueryEnd, GL_QUERY_RESULT, &endTime);

        mGPUTime = endTime - mGPUTime;
    }

    double CarmicahTime::GetGPUTime() const
    {
        return static_cast<double>(mGPUTime) / 1000000.0; // Convert nanoseconds to milliseconds
    }

    // CarmicahTimer namespace implementation
    namespace CarmicahTimer
    {
        void StartTime()
        {
            gCarmicahTime->InitTime();
        }

        void UpdateElapsedTime()
        {
            gCarmicahTime->UpdateTime();
        }

        double GetDt()
        {
            return gCarmicahTime->GetDeltaTime();
        }

        double GetFPS()
        {
            return gCarmicahTime->FPS();
        }

        void StartSystemTimer(const std::string& systemName)
        {
            gCarmicahTime->StartSystemTimer(systemName);
        }

        void StopSystemTimer(const std::string& systemName)
        {
            gCarmicahTime->StopSystemTimer(systemName);
        }

        void StartLoopTimer()
        {
            gCarmicahTime->StartLoopTimer();
        }

        void StopLoopTimer()
        {
            gCarmicahTime->StopLoopTimer();
        }

        void CalculateSystemPercentages()
        {
            gCarmicahTime->CalculateSystemPercentages();
        }

        const std::unordered_map<std::string, double>& GetSystemPercentages()
        {
            return gCarmicahTime->GetSystemPercentages();
        }

        double GetTotalLoopTime()
        {
            return gCarmicahTime->GetTotalLoopTime();
        }

        void InitGPUProfiling()
        {
            gCarmicahTime->InitGPUProfiling();
        }

        void StartGPUTimer()
        {
            gCarmicahTime->StartGPUTimer();
        }

        void StopGPUTimer()
        {
            gCarmicahTime->StopGPUTimer();
        }

        double GetGPUTime()
        {
            return gCarmicahTime->GetGPUTime();
        }
    }
}