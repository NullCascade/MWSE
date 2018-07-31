#pragma once

#include <string>

class ExtendedInputConfig {
public:
	enum InputType {
		ButtonKey,
		Axis
	};

	ExtendedInputConfig(const char* name, short deviceType, short input, short inputType = InputType::ButtonKey);

	~ExtendedInputConfig();

private:
	std::string m_Name;
	short m_DeviceType;
	short m_InputType;
	short m_Input;
};

