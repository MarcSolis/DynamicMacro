#pragma once
#include <string>

#ifndef NDEBUG
// Production builds should set NDEBUG=1
#define NDEBUG false
#else
#define NDEBUG true
#endif

#ifndef DEBUG
#define DEBUG !NDEBUG
#endif

#if DEBUG
#define Error(x) Logger::LogMessage(x, "Error")
#define Warning(x) Logger::LogMessage(x, "Warning")
#define Log(x) Logger::LogMessage(x)
#define Message(x) Logger::ScreenMessage(x)
#else
#define Error(x) 
#define Warning(x) 
#define Log(x) 
#define Message(x)
#endif


class Logger
{
public:
	static void LogMessage(const std::string& message, const std::string& level = "Log");
	static void ScreenMessage(const std::string& message);

private:
	static void GetTime(char* timeBuffer, size_t bufferSize);
	static bool isLogCleaned;
};
