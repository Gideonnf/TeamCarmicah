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


//brief:    this function initializes the log system. It creates a log file with a timestamp and sets up the console logging.
//          It also creates the core and client loggers and sets their log levels to trace.
//          If the log file cannot be created, it logs an error message to the console.
//          If the console logging cannot be set up, it logs an error message to the console.
//          If the log file cannot be opened, it logs an error message to the console.
//          If the log file is opened successfully, it logs a message to the console that the log system has been initialized.
//          If the log file is not opened successfully, it logs an error message to the console.

namespace Carmicah
{
	// Initialize the static variables
	// The static variables are shared pointers to the logger objects
    std::shared_ptr<spdlog::logger> Log::sCoreLogger;
    std::shared_ptr<spdlog::logger> Log::sClientLogger;
    std::vector<std::string> Log::logMessages;
    std::ofstream Log::logFile;
    std::string Log::currentLogFile;

    // Add these static member definitions
    std::ofstream Log::crashLogFile;
    std::string Log::currentCrashLogFile;
    std::vector<std::string> Log::crashMessages;
    bool Log::hasCrashed = false;
	

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

    void Log::initAssertion() {
        try {
            // Create crashes directory if it doesn't exist
            if (!std::filesystem::exists("crashes")) {
                if (!std::filesystem::create_directory("crashes")) {
                    throw std::runtime_error("Failed to create crashes directory");
                }
            }

            // Create crash log file with timestamp
            auto now = std::chrono::system_clock::now();
            auto nowTime = std::chrono::system_clock::to_time_t(now);
            std::stringstream ss;
            ss << "crashes/crash_" << std::put_time(std::localtime(&nowTime), "%Y%m%d_%H%M%S") << ".txt";
            currentCrashLogFile = ss.str();

            // Open crash log file
            crashLogFile.open(currentCrashLogFile, std::ios::out | std::ios::app);
            if (!crashLogFile.is_open()) {
                throw std::runtime_error("Failed to open crash log file: " + currentCrashLogFile);
            }

            // Write header information
            crashLogFile << "Carmicah Engine Crash Log" << std::endl;
            crashLogFile << "=========================" << std::endl;
            crashLogFile << "Generated: " << std::put_time(std::localtime(&nowTime), "%Y-%m-%d %H:%M:%S") << std::endl;
            crashLogFile << std::endl;
        }
        catch (const std::exception& e) {
            UNUSED(e);
            if (sCoreLogger) {
                CM_CORE_ERROR("Failed to initialize crash logging: {}", e.what());
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

    void Log::logCrashMessage(const char* file, int line, const char* function, const char* message) {
		// Log the crash message
		std::string crashMsg = fmt::format("CRASH: {}:{} in function {} - {}", file, line, function, message);
		crashMessages.push_back(crashMsg);

		// Write to crash log file if open
        if (crashLogFile.is_open()) {
			auto now = std::chrono::system_clock::now();
			auto nowTime = std::chrono::system_clock::to_time_t(now);
			crashLogFile << std::put_time(std::localtime(&nowTime), "%Y-%m-%d %H:%M:%S")
				<< " " << crashMsg << std::endl;
		}

		hasCrashed = true;
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

    void Log::writeCrashReport() {
        if (crashLogFile.is_open()) {
            // Write any additional diagnostic information
            crashLogFile << "\nSystem Information:\n";
            // Add relevant system information here

            crashLogFile << "\nFinal log messages before crash:\n";
            // Write the last few log messages that occurred before the crash
            size_t startIdx = logMessages.size() > 10 ? logMessages.size() - 10 : 0;
            for (size_t i = startIdx; i < logMessages.size(); ++i) {
                crashLogFile << logMessages[i] << std::endl;
            }

            crashLogFile.close();
        }
    }

    void Log::flushLogsToFile() {
        if (logFile.is_open()) {
            logFile.flush();
        }
    }
}