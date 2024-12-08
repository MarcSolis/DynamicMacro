#pragma once
#include "InputData.h"
#include <string>

class InputWriter
{
public:
	void SimulateInput(const std::string& commandPath);
	InputContainer LoadInputsFromFile(const std::string& commandPath);
};

