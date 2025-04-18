/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			DebugWindow.cpp

 author:		YANG YUJIE 80%
 co-author:     Lee Yong Yee 20%

 emails:			y.yujie@digipen.edu, l.yongyee@digipen.edu

 brief:         This file contains the implementation of the DebugWindow class
				DebugWindow is a singleton class that is used to display debug information
				on the screen. It is used to display the frame rate, system times, and GPU times.
				It also allows the user to enable and disable the debug window.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#include "pch.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "CarmicahTime.h"
#include "log.h"
//brief:			This DebugWindow class is a derived class from EditorWindow.
//                 It currently provides an FPS tracker, a console log and profiler for tracking purposes.

namespace Carmicah
{
    //CarmicahTime* CarmicahTimer = NULL;

    //CarmicahTime::CarmicahTime()
    //{
    //    if (CarmicahTimer == NULL)
    //        CarmicahTimer = this;
    //}

    //CarmicahTime::~CarmicahTime()
    //{

    //}

    // Call this instead of constructor
    // cause singleton cant define two constructors
    void CarmicahTime::Init()
    {
        //this->mFixedDeltaTime = 0.01667f;
        mUpdateInterval = 0.5;

        std::srand((unsigned int)time(NULL));
    }

    void CarmicahTime::InitTime()
    {
        lastUpdateTime = std::chrono::steady_clock::now();
        mPrevTime = glfwGetTime();
    }

    // Update the time variables and calculate the delta time
    void CarmicahTime::UpdateTime()
    {
        mFrameCount++;

        double currTime = glfwGetTime();
        mDeltaTime = currTime - mPrevTime;
        mPrevTime = currTime;
        mUpdateTimer += mDeltaTime;


        //CM_CORE_INFO("Delta time: {}", mDeltaTime);

        if (mUpdateTimer >= mUpdateInterval)
        {
            mCurrentFPS = static_cast<double>(mFrameCount) / mUpdateTimer;
            mFrameCount = 0;
            mUpdateTimer = 0.0;
        }
    }

    // Get the delta time
    void CarmicahTime::StartSystemTimer(const std::string& systemName)
    {
        mSystemStartTimes[systemName] = std::chrono::steady_clock::now();
    }

    // Get the delta time
    void CarmicahTime::StopSystemTimer(const std::string& systemName)
    {
        auto endTime = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - mSystemStartTimes[systemName]);
        mSystemTimes[systemName] = duration.count() / 1000000.0; // Convert to seconds
    }

    // Get the delta time for the loop timer and calculate the system percentages
    void CarmicahTime::StartLoopTimer()
    {
        mLoopStartTime = std::chrono::steady_clock::now();
    }

    //  Get the delta time for the loop timer and calculate the system percentages
    void CarmicahTime::StopLoopTimer()
    {
        auto endTime = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - mLoopStartTime); // Get the duration in microseconds
        mTotalLoopTime = duration.count() / 1000000.0; // Convert to seconds
    }

    // Get the delta time for the loop timer and calculate the system percentages
    void CarmicahTime::CalculateSystemPercentages()
    {
        for (const auto& pair : mSystemTimes)
        {
            mSystemPercentages[pair.first] = (pair.second / mTotalLoopTime) * 100.0; // Convert to percentage
        }
    }

    // Get the delta time for the loop timer and calculate the system percentages
    void CarmicahTime::InitGPUProfiling()
    {
        // Generate the queries for the GPU profiling timer
        glGenQueries(1, &mGPUQueryStart);
        glGenQueries(1, &mGPUQueryEnd);
    }

    void CarmicahTime::StartGPUTimer() // Start the GPU timer
    {
        glQueryCounter(mGPUQueryStart, GL_TIMESTAMP); // Start the timer by recording a timestamp
    }

    void CarmicahTime::StopGPUTimer() // Stop the GPU timer
    {
        glQueryCounter(mGPUQueryEnd, GL_TIMESTAMP); // Stop the timer by recording a timestamp

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


    /*!*************************************************************************
    \brief
        Generates a random number between min (inclusive) and max (inclusive)
    ***************************************************************************/
    float CarmicahTime::GenerateRandFloat(float min, float max)// eg. 0, 100
    {
        return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX) / (max - min));
    }

    /*!*************************************************************************
    \brief
        Generates a random number between min (inclusive) and max (inclusive)
    ***************************************************************************/
    int CarmicahTime::GenerateRandInt(int min, int max)
    {
        return min + rand() % (max - min);
    }

}