#pragma once

#include "TES3Defines.h"
#include "NIDefines.h"

#include "TES3Collections.h"
#include "TES3Vectors.h"

namespace TES3 {
	namespace WeatherType {
		enum WeatherType {
			Clear,
			Cloudy,
			Foggy,
			Overcast,
			Rain,
			Thunder,
			Ash,
			Blight,
			Snow,
			Blizzard,

			First = Clear,
			Last = Blizzard
		};
	}

	struct WeatherController {
		NI::Node* sgSunVis; // 0x0
		NI::Node* sgSunBase;
		NI::Node* sgSunGlare;
		int daysRemaining; // 0xC
		char unknown_0x10;
		Weather* arrayWeathers[10]; // 0x14
		Weather* currentWeather; // 0x3C
		Weather* nextWeather; // 0x40
		Moon* moonSecunda; // 0x44
		Moon* moonMasser; // 0x48
		NI::Node* sgSkyRoot;
		NI::Node* sgSkyNight;
		NI::Node* sgSkyAtmosphere;
		NI::Node* sgSkyClouds;
		NI::Node* sgRain;
		NI::Node* sgSnow;
		NI::TriShape* sgTriRain;
		NI::Node* sgSnowflake;
		NI::Node* sgStormRoot;
		NI::Node* sgAshCloud;
		NI::Node* sgBlightCloud;
		NI::Node* sgBlizzard;
		NI::TriShape* sgTriAtmosphere;
		NI::TriShape* sgTriClouds0;
		NI::TriShape* sgTriClouds1;
		NI::TriShape* shTriSunBase;
		NI::TriShape* sgTriSunGlare;
		Vector3 currentSkyColor; // 0x90
		Vector3 currentFogColor; // 0x9C
		void* unknown_0xA8;
		void* sgSkyLight;
		int unknown_0xB0;
		int unknown_0xB4;
		Vector3 windVelocityCurrWeather; // 0xB8
		Vector3 windVelocityNextWeather; // 0xC4
		float unknown_0xD0;
		float sunglareFaderMax; // 0xD4
		float sunglareFaderAngleMax; // 0xD8
		float sunriseHour; // 0xDC
		float sunsetHour; // 0xE0
		float sunriseDuration; // 0xE4
		float sunsetDuration; // 0xE8
		float fogDepthChangeSpeed;
		float ambientPreSunriseTime;
		float ambientPostSunriseTime;
		float ambientPreSunsetTime;
		float ambientPostSunsetTime;
		float fogPreSunriseTime;
		float fogPostSunriseTime;
		float fogPreSunsetTime;
		float fogPostSunsetTime;
		float skyPreSunriseTime;
		float skyPostSunriseTime;
		float skyPreSunsetTime;
		float skyPostSunsetTime;
		float sunPreSunriseTime;
		float sunPostSunriseTime;
		float sunPreSunsetTime;
		float sunPostSunsetTime;
		float starsPostSunsetStart;
		float starsPreSunriseFinish;
		float starsFadingDuration;
		int activeRainParticles;
		int activeSnowParticles;
		Iterator<void> listActiveParticles;
		Iterator<void> listInactiveParticles;
		float hoursBetweenWeatherChanges; // 0x16C
		float transitionScalar; // 0x170
		float hoursRemaining; // 0x174
		float unknown_0x178;
		float unknown_0x17C;
		float snowGravityScale;
		float snowHighKill;
		float snowLowKill;
		Vector3 unknown_0x18C;
		char bInactivateWeather;
		char bUnderwaterPitchbendState;
		char bRainRipples;
		char bSnowRipples;
		char bTimescaleClouds; // 0x19C
		float underwaterSunriseFog; // 0x1A0
		float underwaterDayFog; // 0x1A4
		float underwaterSunsetFog; // 0x1A8
		float underwaterNightFog; // 0x1AC
		float underwaterIndoorFog; // 0x1B0
		Vector3 underwaterCol; // 0x1B4
		float underwaterColWeight; // 0x1C0
		Vector3 sunglareFaderCol; // 0x1C4
		Region* lastActiveRegion; // 0x1D0
		DataHandler* dataHandler; // 0x1D4
		Sound* soundUnderwater; // 0x1D8
		Vector3 unknown_0x1DC;
		int unknown_0x1E8;
		int unknown_0x1EC;

		//
		// Other related this-call functions.
		//

		void switchWeather(int weatherId, float startingTransition);
	};
	static_assert(sizeof(WeatherController) == 0x1F0, "TES3::WeatherController failed size validation");
}
