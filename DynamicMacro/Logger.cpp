#include "Logger.h"
#include <windows.h>
#include <fstream>
#include "PathGenerator.h"
#include "Exceptions.h"

#if DEBUG
	#include <iostream>
#endif


	bool Logger::isLogCleaned = false;

	void Logger::LogMessage(const std::string& message, const std::string& level)
	{
		std::string logPath{ PathGenerator::GenerateLogPath() };

		std::ios_base::openmode mode = std::ios::app;

		if (!isLogCleaned)	//Clear logs before write
		{
			mode = std::ios::out;
			isLogCleaned = true;
		}

		std::ofstream logFile(logPath, mode);
		if (!logFile)
		{
#if DEBUG
			std::cerr << "Failed to retrieve file from" << logPath << std::flush;
#endif
			throw Exceptions::ErrorSavingFile{ "Failed to retrieve file from " + logPath };
		}

		char timeBuffer[20];
		GetTime(timeBuffer, sizeof(timeBuffer));

		// Write the message to the log file
		logFile << "[" << timeBuffer << "] [" << level << "] " << message << std::endl;

		// Optionally print the message to the console
		ScreenMessage(message);

		logFile.close();
	}

	void Logger::ScreenMessage(const std::string& message)
	{
#if DEBUG
		std::cout << message << std::endl;
#endif
	}

	void Logger::GetTime(char* timeBuffer, size_t bufferSize)
	{
		std::time_t now = std::time(nullptr);
		struct tm timeInfo;
		if (localtime_s(&timeInfo, &now) != 0)
		{
#if DEBUG
			std::cerr << "Error obtaining current time" << std::endl;
#endif
			return;
		}

		std::strftime(timeBuffer, bufferSize, "%Y-%m-%d %H:%M:%S", &timeInfo);
	}
