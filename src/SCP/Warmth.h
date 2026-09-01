#pragma once

namespace Survival
{
	enum class WarmthClass : std::int32_t
	{
		Normal = 0,
		Warm = 1,
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

		float GetCloakNormalBonus();

		void SetCloakNormalBonus(float warmth);

		float GetCloakWarmBonus();

		void SetCloakWarmBonus(float warmth);

		float GetCloakColdBonus();

		void SetCloakColdBonus(float warmth);

		void SetWarmthOverride(RE::TESObjectARMO* armor, WarmthClass warmthClass);

		void ResetWarmthOverride(RE::TESObjectARMO* armor);

		RE::Setting** GetCloakSettings(WarmthClass warmthClass);

		bool SerializeSave(SKSE::SerializationInterface* a_intfc, std::uint32_t a_type, std::uint32_t a_version);

		bool SerializeSave(SKSE::SerializationInterface* a_intfc);

		bool DeserializeLoad(SKSE::SerializationInterface* a_intfc);

		bool EnableFrostfallKeywords = true;
		bool EnableWarmthForCloaks = true;
		std::unordered_map<RE::FormID, WarmthClass> WarmthOverrides;

	private:
		WarmthSettings()
		{
			_cloakNormalBonus.data = 18.0f;
			_cloakWarmBonus.data = 29.0f;
			_cloakColdBonus.data = 8.0f;
		}

		// Fake game setting to pass to the warmth function. As long as the code does
		// nothing else but read the float, it won't crash.
		struct GameSetting
		{
			std::int64_t _pad_0 = 0;
			float data;
			char* name = nullptr;
		};
		static_assert(sizeof(GameSetting) == sizeof(RE::Setting));

		GameSetting _cloakNormalBonus;
		GameSetting _cloakWarmBonus;
		GameSetting _cloakColdBonus;

		GameSetting* _cloakNormalBonusPtr = &_cloakNormalBonus;
		GameSetting* _cloakWarmBonusPtr = &_cloakWarmBonus;
		GameSetting* _cloakColdBonusPtr = &_cloakColdBonus;
	};
}
