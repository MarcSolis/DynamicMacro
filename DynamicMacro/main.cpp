// Written by Marc Solis (2024)

#include <windows.h>
#include "InputReader.h"
#include "InputWriter.h"
#include "Config.h"
#include "Logger.h"
#include "PathGenerator.h"



enum OperationMode 
{
	READ,
	WRITE
};


int main(int argc, char* argv[])
{
	// try read specified macro name
	std::string macroName{ argc >= 2 ? argv[1] : "macro.bin"};

	PathGenerator pathGenerator(std::move(macroName));

	OperationMode mode{ GetAsyncKeyState(RECORDING_VKC) & 0x8000 ? READ : WRITE };
	Log("Operation mode: " + std::string{ mode == READ ? "READ" : "WRITE" });

	switch (mode)
	{
	case READ:
		InputReader::Getinstance().Activate(pathGenerator.GenerateCommandPath());
		break;

	case WRITE:
		InputWriter inputWriter;
		inputWriter.SimulateInput(pathGenerator.GenerateCommandPath());
		break;

	default:
		return 1;
	}

	return 0;
}