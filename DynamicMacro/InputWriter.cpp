#include "InputWriter.h"
#include <windows.h>
#include <WinUser.h>
#include <fstream>
#include "Config.h"
#include "PathGenerator.h"
#include "Logger.h"


void InputWriter::SimulateInput(const std::string& commandPath)
{
	for (auto inputData : LoadInputsFromFile(commandPath))
	{
		INPUT input{
			INPUT_KEYBOARD,
			inputData.vKCode,
			inputData.inputType == KEY_DOWN ? 0 : KEYEVENTF_KEYUP,
			0,
			0
		};

		SendInput(1, &input, sizeof(INPUT));
	}

}

InputContainer InputWriter::LoadInputsFromFile(const std::string& commandPath)
{
	std::ifstream inFile(commandPath, std::ios::binary);

	if (!inFile)
	{
		Error("File did not exist: " + commandPath);
		return {};
	}

	InputContainer::size_type inputBufferSize;
	inFile.read(reinterpret_cast<char*>(&inputBufferSize), sizeof(inputBufferSize));

	InputContainer inputs;
	inputs.reserve(inputBufferSize);

	for (int i = 0; i < inputBufferSize; ++i)
	{
		InputData inputData;

		// Read the data back from the binary file
		inFile.read(reinterpret_cast<char*>(&inputData.vKCode), sizeof(inputData.vKCode));
		inFile.read(reinterpret_cast<char*>(&inputData.inputType), sizeof(inputData.inputType));
		
		if (inputData.inputType == KEY_DOWN)
		{
			Message("Key pressed: " + (char)inputData.vKCode);
		}
		else
		{
			Message("Key released: " + (char)inputData.vKCode);
		}

		inputs.push_back(inputData);
	}

	inFile.close();

	return inputs;
}
