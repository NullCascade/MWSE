#include "TES3WeatherController.h"

namespace TES3 {
	const auto TES3_WeatherController_switch = reinterpret_cast<void (__thiscall*)(WeatherController*, int, float)>(0x441C40);

	void WeatherController::switchWeather(int weatherId, float startingTransition) {
		TES3_WeatherController_switch(this, weatherId, startingTransition);
	}
}