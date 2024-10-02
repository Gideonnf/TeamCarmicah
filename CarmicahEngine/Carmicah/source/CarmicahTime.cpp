#include "pch.h"
#include "CarmicahTime.h"
#include "log.h"

namespace Carmicah
{
	void CarmicahTime::InitTime()
	{
		lastUpdateTime = std::chrono::steady_clock::now();
		mFrameCount = 0;
		mCurrentFPS = 0.0;
		mPrevTime = glfwGetTime();
	}

	void CarmicahTime::UpdateTime()
	{
		mFrameCount++;

		//auto currentTime = std::chrono::steady_clock::now();
		//mDeltaTime = std::chrono::duration<double>(currentTime - lastUpdateTime).count();

		double currTime = glfwGetTime();
		mDeltaTime = currTime - mPrevTime;
		mPrevTime = currTime;
		mUpdateTimer += mDeltaTime;


		if (mUpdateTimer >= mUpdateInterval)
		{
			mCurrentFPS = static_cast<double>(mFrameCount) / mUpdateTimer;
			mFrameCount = 0;
			//lastUpdateTime = currentTime;
			mUpdateTimer = 0.0;
			// Log the FPS using spdlog
			//auto logger = spdlog::get("CARMICAH");
			//if (logger)
			//{
			//	logger->info("Current FPS: {:.2f}", mCurrentFPS);
			//}
		}
	}

	namespace CarmicahTimer
	{
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
	}
}