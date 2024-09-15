#ifndef TIME_H
#define TIME_H
#include "Singleton.h"
#include <GLFW/glfw3.h>
#include <Windows.h>

namespace Carmicah
{
	class Time : public Singleton<Time>
	{
	public:
		// I dont know if we should use glfwGetTime or window's QueryPerformanceCounter
		// using glfwGetTime cause QueryPerformanceCounter dont work and glfw does so ill just use it for now instead

		//LARGE_INTEdoublGER mPrevTime;
		//LARGE_INTEGER mFrequency;

		double mDeltaTime;
		double mPrevTime;

		Time() : mDeltaTime(0), mPrevTime(0) { /*QueryPerformanceFrequency(&mFrequency);*/ }
		~Time() {}

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
		void StartTime()
		{
			Time::GetInstance()->InitTime();
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

			Time::GetInstance()->UpdateTime();

		}

		double GetDeltaTime()
		{
			return Time::GetInstance()->mDeltaTime;
		}

	}
}


#endif
