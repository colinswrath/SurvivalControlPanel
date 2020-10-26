#include "PapyrusSurvival.h"
#include "Warmth.h"

namespace PapyrusSurvival
{
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

		a_vm->RegisterFunction("SetCloakNormalBonus", "Survival", SetCloakNormalBonus);
		a_vm->RegisterFunction("SetCloakWarmBonus", "Survival", SetCloakWarmBonus);
		a_vm->RegisterFunction("SetCloakColdBonus", "Survival", SetCloakColdBonus);
		a_vm->RegisterFunction("GetCloakNormalBonus", "Survival", GetCloakNormalBonus);
		a_vm->RegisterFunction("GetCloakWarmBonus", "Survival", GetCloakWarmBonus);
		a_vm->RegisterFunction("GetCloakColdBonus", "Survival", GetCloakColdBonus);

		return true;
	}
}