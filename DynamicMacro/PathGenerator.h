#pragma once
#include <string>

class PathGenerator
{
public:
	PathGenerator(const std::string& commandName);
	std::string GenerateCommandPath();
	static std::string GenerateLogPath();


private:
	static std::string GeneratePath(const std::string& specificPath, const std::string& fileName);
	const std::string commandName;
	static std::string logName;
};

