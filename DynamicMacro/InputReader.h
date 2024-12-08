#pragma once
#include "InputData.h"
#include <windows.h>
#include <string>


class InputReader
{
	

public:
	static InputReader& Getinstance();

	void Activate(const std::string& commandPath);
	void InstallInputHook();
	const InputContainer& GetInputBuffer() const;
	void ProcessInput(WPARAM wParam, LPARAM lParam);

	InputReader(const InputReader&) = delete;
	void operator=(const InputReader&) = delete;

	HHOOK& GetHook() { return hHook; }

protected:
	~InputReader();


private:
	void StartReading(const std::string& commandPath);
	void Deactivate();
	void SaveToFile(const std::string& commandPath);

	InputReader();

	HDESK hDesktop; // User session hook
	HHOOK hHook;	// Input binding hook
	InputContainer inputBuffer;
	bool isPendingStart{ true };
};

