/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			log.cpp

 author:		YANG YUJIE 80%
 co-author:		Nicholas Lai 20%

 email:			y.yujie@digipen.edu

 brief:         This file contains the implementation of the log class. 

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#include "pch.h"
#include "log.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <filesystem>


//brief:         This file contains the logging system for the engine. It uses the spdlog library to log messages to the console and to a file.
//				The logging system is used by the engine and the client. The engine uses the core logger and the client uses the client logger.
//				The logging system is disabled in release mode for performance reasons.
namespace Carmicah
{
	// Initialize the static variables
	// The static variables are shared pointers to the logger objects
	std::shared_ptr<spdlog::logger> Log::sCoreLogger;
	std::shared_ptr<spdlog::logger> Log::sClientLogger;
	std::vector<std::string> Log::logMessages;
	std::ofstream Log::logFile;
	std::string Log::currentLogFile;
	

	// Initialize the logger
    void Log::init() {
        try {
            // Try to create logs directory first
            if (!std::filesystem::exists("logs")) {
                if (!std::filesystem::create_directory("logs")) {
                    throw std::runtime_error("Failed to create logs directory");
                }
            }

            // Set up console logging first
            spdlog::set_pattern("%d-%m-%Y %T.%e [%n] %^%l%$ %v");
            sCoreLogger = spdlog::stdout_color_mt("CARMICAH");
            sCoreLogger->set_level(spdlog::level::trace);
            sClientLogger = spdlog::stdout_color_mt("APP");
            sClientLogger->set_level(spdlog::level::trace);

            // Now create log file with timestamp
            auto now = std::chrono::system_clock::now();
            auto nowTime = std::chrono::system_clock::to_time_t(now);
            std::stringstream ss;
            ss << "logs/log_" << std::put_time(std::localtime(&nowTime), "%Y%m%d_%H%M%S") << ".txt";
            currentLogFile = ss.str();

            // Open log file
            logFile.open(currentLogFile, std::ios::out | std::ios::app);
            if (!logFile.is_open()) {
                throw std::runtime_error("Failed to open log file: " + currentLogFile);
            }

            // Log successful initialization
            CM_CORE_INFO("Log system initialized. Writing to {}", currentLogFile);
        }
        catch (const std::exception& e) {
            // If we can't set up file logging, at least log to console
            if (sCoreLogger) {
                CM_CORE_ERROR("Failed to initialize file logging: {}", e.what());
            }
            else {
                std::cerr << "Failed to initialize logging system: " << e.what() << std::endl;
            }
        }
    }

    void Log::logMessage(const std::string& msg) {
        logMessages.push_back(msg);

        // Write to file if open
        if (logFile.is_open()) {
            auto now = std::chrono::system_clock::now();
            auto nowTime = std::chrono::system_clock::to_time_t(now);
            logFile << std::put_time(std::localtime(&nowTime), "%Y-%m-%d %H:%M:%S")
                << " " << msg << std::endl;

            // Flush periodically to avoid data loss
            if (logMessages.size() % LOG_FLUSH_THRESHOLD == 0) {
                flushLogsToFile();
            }
        }
    }

    void Log::writeLogsToFile(const std::string& filename) {
        std::ofstream outFile(filename);
        if (!outFile.is_open()) {
            CM_CORE_ERROR("Failed to open file for writing logs: {}", filename);
            return;
        }

        for (const auto& msg : logMessages) {
            outFile << msg << std::endl;
        }
        outFile.close();
    }

    void Log::flushLogsToFile() {
        if (logFile.is_open()) {
            logFile.flush();
        }
    }
}