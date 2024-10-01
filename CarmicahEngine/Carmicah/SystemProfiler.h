#pragma once
// In a new file, say SystemProfiler.h
#include <chrono>
#include <unordered_map>
#include <string>

class SystemProfiler
{
public:
    static void StartProfile(const std::string& systemName)
    {
        startTimes[systemName] = std::chrono::high_resolution_clock::now();
    }

    static void EndProfile(const std::string& systemName)
    {
        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTimes[systemName]);
        systemTimes[systemName] = duration;
    }

    static std::chrono::nanoseconds GetSystemTime(const std::string& systemName)
    {
        return systemTimes[systemName];
    }

    static std::chrono::nanoseconds GetTotalTime()
    {
        std::chrono::nanoseconds total{ 0 };
        for (const auto& pair : systemTimes)
        {
            total += pair.second;
        }
        return total;
    }

private:
    static std::unordered_map<std::string, std::chrono::high_resolution_clock::time_point> startTimes;
    static std::unordered_map<std::string, std::chrono::nanoseconds> systemTimes;
};