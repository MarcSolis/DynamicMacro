#include "InputReader.h"
#include "Config.h"
#include "Exceptions.h"
#include "PathGenerator.h"
#include <fstream>
#include "Logger.h"


// Low-level keyboard hook procedure
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	auto& hHook = InputReader::Getinstance().GetHook();

	if (nCode == HC_ACTION)
	{
		InputReader::Getinstance().ProcessInput(wParam, lParam);

		// Return the hook to pass the event to other hooks (or the system)
		return CallNextHookEx(hHook, nCode, wParam, lParam);
	}

	return CallNextHookEx(hHook, nCode, wParam, lParam);
}


InputReader::InputReader()
{
	inputBuffer.reserve(MAX_BUFFER_SIZE / 2);
}

InputReader& InputReader::Getinstance()
{
	static InputReader instance;
	return instance;
}

void InputReader::Activate(const std::string& commandPath)
{
	InstallInputHook();
	StartReading(commandPath);
}

void InputReader::InstallInputHook()
{
	// Open the input desktop
	hDesktop = OpenInputDesktop(DF_ALLOWOTHERACCOUNTHOOK, FALSE, DESKTOP_HOOKCONTROL);
	if (!hDesktop)
	{
		Error("Failed to open input desktop");
		throw Exceptions::InvalidInputDesktop{ "Failed to open input desktop. Error: " + GetLastError() };
	}
	else
	{
		Log("Opened Input Desktop");
	}

	// Set the thread desktop to the input desktop
	if (!SetThreadDesktop(hDesktop))
	{
		Error("Failed to set thread desktop");
		CloseDesktop(hDesktop);
		//throw Exceptions::InvalidDesktopThread{ "Failed to set thread desktop. Error: " + GetLastError() };
	}
	else
	{
		Log("Thread set to Desktop");
	}

	// Install low-level keyboard hook (Admin privileges & user Session required)
	hHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, nullptr, 0);
	if (hHook == nullptr)
	{
		Error("Failed to install input hook");
		CloseDesktop(hDesktop);
		throw Exceptions::InvalidInputHook{ "Failed to install input hook. Error:" + GetLastError() };
	}

	Log("Global keyboard hooked. Press 'Esc' to exit");
}

const InputContainer& InputReader::GetInputBuffer() const
{
	return inputBuffer;
}

void InputReader::ProcessInput(WPARAM wParam, LPARAM lParam)
{
	if (inputBuffer.size() == MAX_BUFFER_SIZE)
	{
		Warning("Max buffer size!");
		return;
	}

	if (isPendingStart)
	{
		if (GetAsyncKeyState(RECORDING_VKC) & 0x8000)
		{
			return;
		}
		
		isPendingStart = false;
	}

	KBDLLHOOKSTRUCT* pKeyboard = (KBDLLHOOKSTRUCT*)lParam;

	if (pKeyboard->vkCode == EXIT_VKC)
	{
		Warning("Exit key detected. Stop recording...");
		return;
	}

	if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
	{
		// Print the character pressed (this works for standard ASCII keys)
		Message("Key pressed: " + (char)pKeyboard->vkCode);
		inputBuffer.emplace_back(static_cast<uint8_t>(pKeyboard->vkCode), InputType::KEY_DOWN);
	}
	else if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP) 
	{
		// Print the character pressed (this works for standard ASCII keys)
		Message("Key released: " + (char)pKeyboard->vkCode);
		inputBuffer.emplace_back(static_cast<uint8_t>(pKeyboard->vkCode), InputType::KEY_UP);
	}
}

InputReader::~InputReader()
{
	Deactivate();
}

void InputReader::StartReading(const std::string& commandPath)
{
	// Message loop to keep the application alive and processing events
	MSG msg;
	while (true)
	{
		// Process any messages from the event queue
		auto result = PeekMessage(&msg, nullptr, WM_KEYFIRST, WM_KEYLAST, PM_NOREMOVE);

		if (result == -1)
		{
			Error("Unexpected exiting. Result: " + (int)result);
			break;
		}
		else if ((GetAsyncKeyState(EXIT_VKC) & 0x8000))
		{
			Warning("Esc key detected. Exiting...");
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			break;
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	
	Deactivate();

	//Save buffer
	if (!inputBuffer.empty())
	{
		SaveToFile(commandPath);
	}
}

void InputReader::Deactivate()
{
	Log("Deactivating reader");
	// Unhook the keyboard hook when done
	UnhookWindowsHookEx(hHook);
	CloseDesktop(hDesktop);
}

void InputReader::SaveToFile(const std::string& commandPath)
{
	// Open a binary file for writing
	std::ofstream outFile(commandPath, std::ios::binary);

	if (!outFile)
	{
		Error("Failed to retrieve file from" + commandPath);
		throw Exceptions::ErrorSavingFile{ "Failed to retrieve file from" + commandPath };
	}

	InputContainer::size_type bufferSize{ inputBuffer.size() };
	outFile.write(reinterpret_cast<const char*>(&bufferSize), sizeof(bufferSize));

	for (auto&& inputData : inputBuffer)
	{
		// Write the data as raw binary (vKCode and inputType)
		outFile.write(reinterpret_cast<const char*>(&inputData.vKCode), sizeof(inputData.vKCode));
		outFile.write(reinterpret_cast<const char*>(&inputData.inputType), sizeof(inputData.inputType));
	}

	outFile.close();
	Warning("Command saved into " + commandPath);
}
