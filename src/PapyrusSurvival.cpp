#include "PapyrusSurvival.h"

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

		return true;
	}
}