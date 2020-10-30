#include "PapyrusSurvival.h"
#include "Warmth.h"
#include "version.h"

namespace PapyrusSurvival
{
	int GetVersion(RE::StaticFunctionTag*)
	{
		return SRCP_VERSION_MAJOR;
	}

	int GetVersionMinor(RE::StaticFunctionTag*)
	{
		return SRCP_VERSION_MINOR;
	}

	void ForceEnable(RE::StaticFunctionTag*, Feature a_feature)
	{
		auto settings = Survival::GetSettings(a_feature);
		if (!settings)
		{
			return;
		}

		settings->ForceEnable();
	}

	void RequestDisable(RE::StaticFunctionTag*, Feature a_feature)
	{
		auto settings = Survival::GetSettings(a_feature);
		if (!settings)
		{
			return;
		}

		settings->RequestDisable();
	}

	void ModReset(RE::StaticFunctionTag*, Feature a_feature)
	{
		auto settings = Survival::GetSettings(a_feature);
		if (!settings)
		{
			return;
		}

		settings->ModReset();
	}

	void UserEnable(RE::StaticFunctionTag*, Feature a_feature)
	{
		auto settings = Survival::GetSettings(a_feature);
		if (!settings)
		{
			return;
		}

		settings->UserEnable();
	}

	void UserDisable(RE::StaticFunctionTag*, Feature a_feature)
	{
		auto settings = Survival::GetSettings(a_feature);
		if (!settings)
		{
			return;
		}

		settings->UserDisable();
	}

	void UserReset(RE::StaticFunctionTag*, Feature a_feature)
	{
		auto settings = Survival::GetSettings(a_feature);
		if (!settings)
		{
			return;
		}

		settings->UserReset();
	}

	bool IsEnabledByMods(RE::StaticFunctionTag*, Feature a_feature)
	{
		auto settings = Survival::GetSettings(a_feature);
		if (!settings)
		{
			return false;
		}

		return settings->ModPreference == Survival::Preference::AlwaysEnabled;
	}

	bool IsDisabledByMods(RE::StaticFunctionTag*, Feature a_feature)
	{
		auto settings = Survival::GetSettings(a_feature);
		if (!settings)
		{
			return false;
		}

		return settings->ModPreference == Survival::Preference::AlwaysDisabled;
	}

	bool IsEnabledByUser(RE::StaticFunctionTag*, Feature a_feature)
	{
		auto settings = Survival::GetSettings(a_feature);
		if (!settings)
		{
			return false;
		}

		return settings->UserPreference == Survival::Preference::AlwaysEnabled;
	}

	bool IsDisabledByUser(RE::StaticFunctionTag*, Feature a_feature)
	{
		auto settings = Survival::GetSettings(a_feature);
		if (!settings)
		{
			return false;
		}

		return settings->UserPreference == Survival::Preference::AlwaysDisabled;
	}

	void EnableFrostfallKeywords(RE::StaticFunctionTag*)
	{
		auto& settings = Survival::WarmthSettings::GetSingleton();
		settings.EnableFrostfallKeywords = true;
	}

	void DisableFrostfallKeywords(RE::StaticFunctionTag*)
	{
		auto& settings = Survival::WarmthSettings::GetSingleton();
		settings.EnableFrostfallKeywords = false;
	}

	bool AreFrostfallKeywordsEnabled(RE::StaticFunctionTag*)
	{
		auto& settings = Survival::WarmthSettings::GetSingleton();
		return settings.EnableFrostfallKeywords;
	}

	void EnableCloakWarmth(RE::StaticFunctionTag*)
	{
		auto& settings = Survival::WarmthSettings::GetSingleton();
		settings.EnableWarmthForCloaks = true;
	}

	void DisableCloakWarmth(RE::StaticFunctionTag*)
	{
		auto& settings = Survival::WarmthSettings::GetSingleton();
		settings.EnableWarmthForCloaks = false;
	}

	bool IsCloakWarmthEnabled(RE::StaticFunctionTag*)
	{
		auto& settings = Survival::WarmthSettings::GetSingleton();
		return settings.EnableWarmthForCloaks;
	}

	void SetCloakNormalBonus(RE::StaticFunctionTag*, float a_warmth)
	{
		auto& settings = Survival::WarmthSettings::GetSingleton();
		settings.SetCloakNormalBonus(a_warmth);
	}

	void SetCloakWarmBonus(RE::StaticFunctionTag*, float a_warmth)
	{
		auto& settings = Survival::WarmthSettings::GetSingleton();
		settings.SetCloakWarmBonus(a_warmth);
	}

	void SetCloakColdBonus(RE::StaticFunctionTag*, float a_warmth)
	{
		auto& settings = Survival::WarmthSettings::GetSingleton();
		settings.SetCloakColdBonus(a_warmth);
	}

	float GetCloakNormalBonus(RE::StaticFunctionTag*)
	{
		auto& settings = Survival::WarmthSettings::GetSingleton();
		return settings.GetCloakNormalBonus();
	}

	float GetCloakWarmBonus(RE::StaticFunctionTag*)
	{
		auto& settings = Survival::WarmthSettings::GetSingleton();
		return settings.GetCloakWarmBonus();
	}

	float GetCloakColdBonus(RE::StaticFunctionTag*)
	{
		auto& settings = Survival::WarmthSettings::GetSingleton();
		return settings.GetCloakColdBonus();
	}

	void SetArmorWarmthNormal(RE::StaticFunctionTag*, RE::TESObjectARMO* a_armor)
	{
		auto& settings = Survival::WarmthSettings::GetSingleton();
		settings.SetWarmthOverride(a_armor, Survival::WarmthClass::Normal);
	}

	void SetArmorWarmthWarm(RE::StaticFunctionTag*, RE::TESObjectARMO* a_armor)
	{
		auto& settings = Survival::WarmthSettings::GetSingleton();
		settings.SetWarmthOverride(a_armor, Survival::WarmthClass::Warm);
	}

	void SetArmorWarmthCold(RE::StaticFunctionTag*, RE::TESObjectARMO* a_armor)
	{
		auto& settings = Survival::WarmthSettings::GetSingleton();
		settings.SetWarmthOverride(a_armor, Survival::WarmthClass::Cold);
	}

	void ResetArmorWarmthToDefault(RE::StaticFunctionTag*, RE::TESObjectARMO* a_armor)
	{
		auto& settings = Survival::WarmthSettings::GetSingleton();
		settings.ResetWarmthOverride(a_armor);
	}

	void ResetAllArmorWarmthToDefault(RE::StaticFunctionTag*)
	{
		auto& settings = Survival::WarmthSettings::GetSingleton();
		settings.WarmthOverrides.clear();
	}


	bool RegisterFuncs(VM* a_vm)
	{
		a_vm->RegisterFunction("ForceEnable", "Survival", ForceEnable);
		a_vm->RegisterFunction("RequestDisable", "Survival", RequestDisable);
		a_vm->RegisterFunction("ModReset", "Survival", ModReset);
		a_vm->RegisterFunction("UserEnable", "Survival", UserEnable);
		a_vm->RegisterFunction("UserDisable", "Survival", UserDisable);
		a_vm->RegisterFunction("UserReset", "Survival", UserReset);

		a_vm->RegisterFunction("IsEnabledByMods", "Survival", IsEnabledByMods);
		a_vm->RegisterFunction("IsDisabledByMods", "Survival", IsDisabledByMods);
		a_vm->RegisterFunction("IsEnabledByUser", "Survival", IsEnabledByUser);
		a_vm->RegisterFunction("IsDisabledByUser", "Survival", IsDisabledByUser);

		a_vm->RegisterFunction("EnableFrostfallKeywords", "Survival", EnableFrostfallKeywords);
		a_vm->RegisterFunction("DisableFrostfallKeywords", "Survival", DisableFrostfallKeywords);
		a_vm->RegisterFunction("AreFrostfallKeywordsEnabled", "Survival", AreFrostfallKeywordsEnabled);

		a_vm->RegisterFunction("EnableCloakWarmth", "Survival", EnableCloakWarmth);
		a_vm->RegisterFunction("DisableCloakWarmth", "Survival", DisableCloakWarmth);
		a_vm->RegisterFunction("IsCloakWarmthEnabled", "Survival", IsCloakWarmthEnabled);

		a_vm->RegisterFunction("SetCloakNormalBonus", "Survival", SetCloakNormalBonus);
		a_vm->RegisterFunction("SetCloakWarmBonus", "Survival", SetCloakWarmBonus);
		a_vm->RegisterFunction("SetCloakColdBonus", "Survival", SetCloakColdBonus);

		a_vm->RegisterFunction("GetCloakNormalBonus", "Survival", GetCloakNormalBonus);
		a_vm->RegisterFunction("GetCloakWarmBonus", "Survival", GetCloakWarmBonus);
		a_vm->RegisterFunction("GetCloakColdBonus", "Survival", GetCloakColdBonus);

		a_vm->RegisterFunction("SetArmorWarmthNormal", "Survival", SetArmorWarmthNormal);
		a_vm->RegisterFunction("SetArmorWarmthWarm", "Survival", SetArmorWarmthWarm);
		a_vm->RegisterFunction("SetArmorWarmthCold", "Survival", SetArmorWarmthCold);
		a_vm->RegisterFunction("ResetArmorWarmthToDefault", "Survival", ResetArmorWarmthToDefault);
		a_vm->RegisterFunction("ResetAllArmorWarmthToDefault", "Survival", ResetAllArmorWarmthToDefault);

		return true;
	}
}