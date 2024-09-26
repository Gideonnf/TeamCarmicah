#ifndef CARMICAH_TIME_H
#define CARMICAH_TIME_H
#include "Singleton.h"
#include <GLFW/glfw3.h>
#include <Windows.h>
#include <chrono>

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
		

	public:
		// I dont know if we should use glfwGetTime or window's QueryPerformanceCounter
		// using glfwGetTime cause QueryPerformanceCounter dont work and glfw does so ill just use it for now instead

		//LARGE_INTEdoublGER mPrevTime;
		//LARGE_INTEGER mFrequency;

		CarmicahTime() : mDeltaTime(0), mUpdateTimer(0.0), mUpdateInterval(0.5), mFrameCount(0), mCurrentFPS(0.0) {}
		~CarmicahTime() {}

		void InitTime();

		void UpdateTime();

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

	}
}


#endif
