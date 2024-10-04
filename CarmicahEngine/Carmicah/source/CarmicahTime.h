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
        std::unordered_map<std::string, std::chrono::steady_clock::time_point> mSystemStartTimes;
        std::unordered_map<std::string, double> mSystemTimes;
        std::unordered_map<std::string, double> mSystemPercentages;
        double mTotalLoopTime;
        std::chrono::steady_clock::time_point mLoopStartTime;
        GLuint mGPUQueryStart;
        GLuint mGPUQueryEnd;
        GLuint64 mGPUTime;

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
        double GetGPUTime() const;
        double FPS() const { return mCurrentFPS; }
        double GetDeltaTime() const { return mDeltaTime; }
        const std::unordered_map<std::string, double>& GetSystemPercentages() const { return mSystemPercentages; }
        double GetTotalLoopTime() const { return mTotalLoopTime; }
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
    }
}

#endif