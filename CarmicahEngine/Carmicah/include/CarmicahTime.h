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
		static std::chrono::steady_clock::time_point start_time;
		static std::chrono::steady_clock::time_point last_frame_time;
		static double delta_time;


	public:
		// I dont know if we should use glfwGetTime or window's QueryPerformanceCounter
		// using glfwGetTime cause QueryPerformanceCounter dont work and glfw does so ill just use it for now instead

		//LARGE_INTEdoublGER mPrevTime;
		//LARGE_INTEGER mFrequency;

		static void Start();
		static void UpdateElapsedTime();
		static double GetDeltaTime();
		static double GetTime();

		double mDeltaTime;
		double mPrevTime;

		CarmicahTime() : mDeltaTime(0), mPrevTime(0) { /*QueryPerformanceFrequency(&mFrequency);*/ }
		~CarmicahTime() {}

		void InitTime()
		{
			mPrevTime = glfwGetTime();
		}

		void UpdateTime()
		{
			double currTime = glfwGetTime();
			mDeltaTime = currTime - mPrevTime;
			mPrevTime = currTime;
		}
	};

	namespace CarmicahTimer
	{
		static CarmicahTime timerObj;

		void StartTime()
		{
			timerObj.InitTime();
			//Time::GetInstance()->
			//QueryPerformanceCounter(&Time::GetInstance()->mPrevTime);
		}

		void UpdateElapsedTime()
		{
			//LARGE_INTEGER currTime;
			//// Get the current time
			//QueryPerformanceCounter(&currTime);
			//// Current time - prev time divided by frequency and cast into double to get delta time
			//Time::GetInstance()->mDeltaTime = static_cast<double>((currTime.QuadPart - Time::GetInstance()->mPrevTime.QuadPart) / Time::GetInstance()->mFrequency.QuadPart);
			//// Update the prev time
			//Time::GetInstance()->mPrevTime = currTime;

			timerObj.UpdateTime();

		}

		double GetDeltaTime()
		{
			return timerObj.mDeltaTime;
		}

	}
}


#endif
