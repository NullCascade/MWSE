#include "LuaExtendedInputConfig.h"

ExtendedInputConfig::ExtendedInputConfig(const char* name, short deviceType, short input, short inputType) :
	m_Name(name),
	m_DeviceType(deviceType),
	m_InputType(inputType),
	m_Input(input)
{
	
}

ExtendedInputConfig::~ExtendedInputConfig()
{

}
