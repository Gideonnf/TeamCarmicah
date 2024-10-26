/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			DebugWindow.h

 author:		YANG YUJIE 80%
 co-author(s): Gideon Francis 20%

 email:			y.yujie@digipen.edu

 brief:         This file contains the declaration of the CarmicahTime class. This class is a singleton that is used to keep track of the time in the engine.
				It is used to calculate the delta time, frames per second, and the time taken by each system in the engine.
				It also contains functions to start and stop timers for each system and the loop, as well as functions to calculate the percentage of time taken by each system.
				It also contains functions to start and stop GPU timers and calculate the time taken by the GPU.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef CARMICAH_TIME_H
#define CARMICAH_TIME_H

#include <chrono>
#include <unordered_map>
#include <string>
#include "Singleton.h"
#include <deque>

// Forward declare GLuint and GLuint64
typedef unsigned int GLuint;
typedef unsigned long long GLuint64;

// Namespace for the engine code    
//brief:         This class is a singleton that is used to keep track of the time in the engine.
//                 It is used to calculate the delta time, frames per second, and the time taken by each system in the engine.
//                 It also contains functions to start and stop timers for each system and the loop, as well as functions to calculate the percentage of time taken by each system.
//                 It also contains functions to start and stop GPU timers and calculate the time taken by the GPU.
namespace Carmicah
{
    struct SystemTimingData {
        double startTime;
        double duration;
        double percentage;
    };

    struct FrameData {
        double frameTime;
        std::unordered_map<std::string, SystemTimingData> systemTimings;
        double cpuUsage;
        double gpuTime;
    };

// Class to keep track of the time in the engine 
    class CarmicahTime : public Singleton<CarmicahTime> // Inherit from Singleton
    {
    private:

        std::chrono::steady_clock::time_point lastUpdateTime;
        double mUpdateTimer;
        double mUpdateInterval;
        int mFrameCount;
        double mCurrentFPS;
        double mDeltaTime;
        double mPrevTime;

        // Profiling variables
        std::unordered_map<std::string, std::chrono::steady_clock::time_point> mSystemStartTimes;
        std::unordered_map<std::string, double> mSystemTimes;
        std::unordered_map<std::string, double> mSystemPercentages;
        double mTotalLoopTime;
        std::chrono::steady_clock::time_point mLoopStartTime;

        // GPU Profiling variables
        GLuint mGPUQueryStart;
        GLuint mGPUQueryEnd;
        GLuint64 mGPUTime;

        //Frame History
        std::deque<FrameData> mFrameHistory;
        static const size_t MAX_FRAME_HISTORY = 300; //Should be enough to show a few seconds of history

        double mCPUUsage{ 0.0 }; // Add CPU usage tracking


    public:
        //CarmicahTime(); // public constructor
        //~CarmicahTime(); // Public destructor
        void Init();
        void InitTime();
        void UpdateTime();
        void StartSystemTimer(const std::string& systemName);
        void StopSystemTimer(const std::string& systemName);
        void StartLoopTimer();
        void StopLoopTimer();
        void CalculateSystemPercentages();
        void InitGPUProfiling();
        void StartGPUTimer();
        void StopGPUTimer();

        // New methods
        const std::deque<FrameData>& GetFrameHistory() const { return mFrameHistory; }
        void UpdateFrameData();
        double GetGPUTime() const;

        // Existing getters
        double FPS() const { return mCurrentFPS; }
        double GetDeltaTime() const { return mDeltaTime; }
        const std::unordered_map<std::string, double>& GetSystemPercentages() const { return mSystemPercentages; }
        double GetTotalLoopTime() const { return mTotalLoopTime; }
        double GetGPUTime() const;

        void UpdateCPUUsage();
        double GetCPUUsage() const { return mCPUUsage; }
        void UpdateCPUUsage(); // New method to update CPU usage


    };

    // Global accessor
    static CarmicahTime& gCTimer = *CarmicahTime::GetInstance();

// Namespace for static functions
    namespace CarmicahTimer
    {
        void StartTime();
        void UpdateElapsedTime();
        double GetDt();
        double GetFPS();
        void StartSystemTimer(const std::string& systemName);
        void StopSystemTimer(const std::string& systemName);
        void StartLoopTimer();
        void StopLoopTimer();
        void CalculateSystemPercentages();
        const std::unordered_map<std::string, double>& GetSystemPercentages();
        double GetTotalLoopTime();
        void InitGPUProfiling();
        void StartGPUTimer();
        void StopGPUTimer();
        double GetGPUTime();
        const std::deque<FrameData>& GetFrameHistory();
        double GetCPUUsage();


    }
}

#endif