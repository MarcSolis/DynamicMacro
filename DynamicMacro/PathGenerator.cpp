#include "PathGenerator.h"
#include "Config.h"
#include "Exceptions.h"
#include <direct.h> 
#include "Logger.h"

std::string PathGenerator::logName = "Log.txt";

PathGenerator::PathGenerator(const std::string& commandNameParam) :
	commandName{ commandNameParam }
{
	Log("Macro name is: " + commandName);
}

std::string PathGenerator::GenerateCommandPath()
{
	return GeneratePath(SAVE_PATH, commandName);
}

std::string PathGenerator::GenerateLogPath()
{
	return GeneratePath(LOGS_PATH, logName);
}

std::string PathGenerator::GeneratePath(const std::string& specificPath, const std::string& fileName)
{
	std::string root{""};

	char buffer[MAX_PATH];
	if (GetModuleFileNameA(NULL, buffer, MAX_PATH) == 0)
	{
		throw Exceptions::InvalidExecutablePath{ "Executable path not found" };
	}
	std::string executablePath(buffer);

	// Find the last backslash, which precedes the file name
	size_t pos = executablePath.find_last_of("\\");
	if (pos != std::string::npos)
	{
		root = executablePath.substr(0, pos + 1);
	}

	root = { root + specificPath + fileName };

	char bufferCD[512];
	if (_getcwd(bufferCD, sizeof(bufferCD)) != nullptr)
	{
		Message("Current working directory: " + std::string(bufferCD));
	}

	return root;
}
