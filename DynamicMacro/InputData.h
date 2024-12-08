#pragma once
#include <cstdint>
#include <vector>

enum InputType : bool
{
	KEY_DOWN,
	KEY_UP
};

struct InputData 
{
	uint8_t vKCode;
	InputType inputType;

	InputData() = default;

	InputData(uint8_t code, InputType type) : vKCode(code), inputType(type)
	{
	}
};

using InputContainer = std::vector<InputData>;