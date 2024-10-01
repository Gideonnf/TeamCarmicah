#ifndef CARMICAH_TIME_H
#define CARMICAH_TIME_H

#include <chrono>
#include <unordered_map>
#include <string>

// Forward declare GLuint and GLuint64
typedef unsigned int GLuint;
typedef unsigned long long GLuint64;

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

        std::unordered_map<std::string, double> mSystemTimes;
        std::unordered_map<std::string, double> mSystemPercentages;
        double mTotalLoopTime;

        GLuint mGPUQueryStart;
        GLuint mGPUQueryEnd;
        GLuint64 mGPUTime;

    public:
        inline CarmicahTime()
            : mUpdateTimer(0.0), mUpdateInterval(0.5), mFrameCount(0), mCurrentFPS(0.0),
            mDeltaTime(0), mPrevTime(0), mTotalLoopTime(0.0),
            mGPUQueryStart(0), mGPUQueryEnd(0), mGPUTime(0)
        {}

        inline ~CarmicahTime()
        {
            if (mGPUQueryStart)
                glDeleteQueries(1, &mGPUQueryStart);
            if (mGPUQueryEnd)
                glDeleteQueries(1, &mGPUQueryEnd);
        }

        inline void InitTime()
        {
            lastUpdateTime = std::chrono::steady_clock::now();
            mFrameCount = 0;
            mCurrentFPS = 0.0;
            mPrevTime = glfwGetTime();
        }

        inline void UpdateTime()
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

        inline void StartSystemTimer(const std::string& systemName)
        {
            if (!systemName.empty()) {
                mSystemTimes[systemName] = glfwGetTime();
            }
        }

        inline void StopSystemTimer(const std::string& systemName)
        {
            if (!systemName.empty() && mSystemTimes.find(systemName) != mSystemTimes.end()) {
                double endTime = glfwGetTime();
                mSystemTimes[systemName] = endTime - mSystemTimes[systemName];
            }
        }

        inline void StartLoopTimer() { mTotalLoopTime = glfwGetTime(); }

        inline void StopLoopTimer()
        {
            double endTime = glfwGetTime();
            mTotalLoopTime = endTime - mTotalLoopTime;
        }

        inline void CalculateSystemPercentages()
        {
            if (mTotalLoopTime > 0) {
                for (const auto& [systemName, systemTime] : mSystemTimes)
                {
                    mSystemPercentages[systemName] = (systemTime / mTotalLoopTime) * 100.0;
                }
            }
        }

        inline void InitGPUProfiling()
        {
            glGenQueries(1, &mGPUQueryStart);
            glGenQueries(1, &mGPUQueryEnd);
        }

        inline void StartGPUTimer() { glQueryCounter(mGPUQueryStart, GL_TIMESTAMP); }

        inline void StopGPUTimer()
        {
            glQueryCounter(mGPUQueryEnd, GL_TIMESTAMP);

            GLint done = 0;
            while (!done)
            {
                glGetQueryObjectiv(mGPUQueryEnd, GL_QUERY_RESULT_AVAILABLE, &done);
            }

            GLuint64 startTime, endTime;
            glGetQueryObjectui64v(mGPUQueryStart, GL_QUERY_RESULT, &startTime);
            glGetQueryObjectui64v(mGPUQueryEnd, GL_QUERY_RESULT, &endTime);
            mGPUTime = endTime - startTime;
        }

        inline double GetGPUTime() const { return static_cast<double>(mGPUTime) / 1000000.0; }
        inline double FPS() const { return mCurrentFPS; }
        inline double GetDeltaTime() const { return mDeltaTime; }
        inline const std::unordered_map<std::string, double>& GetSystemPercentages() const { return mSystemPercentages; }
        inline double GetTotalLoopTime() const { return mTotalLoopTime; }
    };
}

#endif