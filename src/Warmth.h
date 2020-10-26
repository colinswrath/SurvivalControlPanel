#pragma once

#include "Offsets.h"

namespace Survival
{
	enum class WarmthClass : int32_t
	{
		Warm = 0,
		Normal = 1,
		Cold = 2,
	};

	class WarmthSettings
	{
	public:
		static WarmthSettings& GetSingleton()
		{
			static WarmthSettings settings;
			return settings;
		}

		float GetCloakWarmBonus()
		{
			return _cloakWarmBonus.data;
		}

		void SetCloakWarmBonus(float warmth)
		{
			_cloakWarmBonus.data = warmth;
		}

		float GetCloakNormalBonus()
		{
			return _cloakNormalBonus.data;
		}

		void SetCloakNormalBonus(float warmth)
		{
			_cloakNormalBonus.data = warmth;
		}

		float GetCloakColdBonus()
		{
			return _cloakColdBonus.data;
		}

		void SetCloakColdBonus(float warmth)
		{
			_cloakColdBonus.data = warmth;
		}

		RE::Setting** GetCloakSettings(WarmthClass warmthClass)
		{
			// Actually, the game expects an array of 4 pointers, but if we set
			// the "slot mask" to 0x1, it will only read one.
			switch (warmthClass) {
			case WarmthClass::Warm:
				return reinterpret_cast<RE::Setting**>(&_cloakWarmBonusPtr);
			case WarmthClass::Cold:
				return reinterpret_cast<RE::Setting**>(&_cloakColdBonusPtr);
			default:
				return reinterpret_cast<RE::Setting**>(&_cloakNormalBonusPtr);
			}
		}

		bool EnableFrostfallKeywords = true;
		bool EnableWarmthForCloaks = true;
		std::map<RE::FormID, WarmthClass> WarmthOverrides;

	private:
		WarmthSettings()
		{
			_cloakWarmBonus.data = 29.0f;
			_cloakNormalBonus.data = 18.0f;
			_cloakColdBonus.data = 8.0f;
		}

		// Fake game setting to pass to the warmth function. As long as the code does
		// nothing else but read the float, it won't crash.
		struct GameSetting
		{
			int64_t _pad_0 = 0;
			float data;
			char* name = nullptr;
		};
		static_assert(sizeof(GameSetting) == sizeof(RE::Setting));

		GameSetting _cloakWarmBonus;
		GameSetting _cloakNormalBonus;
		GameSetting _cloakColdBonus;

		GameSetting* _cloakWarmBonusPtr = &_cloakWarmBonus;
		GameSetting* _cloakNormalBonusPtr = &_cloakNormalBonus;
		GameSetting* _cloakColdBonusPtr = &_cloakColdBonus;
	};
}
