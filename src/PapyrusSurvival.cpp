#include "PapyrusSurvival.h"

namespace PapyrusSurvival
{
	void ForceEnable(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, Feature a_feature)
	{
		auto settings = Survival::GetSettings(a_feature);
		if (!settings)
		{
			a_vm->TraceStack("aiFeature is not a valid Survival feature", a_stackID, Severity::kWarning);
			return;
		}

		settings->ForceEnable();
	}

	void RequestDisable(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, Feature a_feature)
	{
		auto settings = Survival::GetSettings(a_feature);
		if (!settings)
		{
			a_vm->TraceStack("aiFeature is not a valid Survival feature", a_stackID, Severity::kWarning);
			return;
		}

		settings->RequestDisable();
	}

	void ModReset(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, Feature a_feature)
	{
		auto settings = Survival::GetSettings(a_feature);
		if (!settings)
		{
			a_vm->TraceStack("aiFeature is not a valid Survival feature", a_stackID, Severity::kWarning);
			return;
		}

		settings->ModReset();
	}

	void UserEnable(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, Feature a_feature)
	{
		auto settings = Survival::GetSettings(a_feature);
		if (!settings)
		{
			a_vm->TraceStack("aiFeature is not a valid Survival feature", a_stackID, Severity::kWarning);
			return;
		}

		settings->UserEnable();
	}

	void UserDisable(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, Feature a_feature)
	{
		auto settings = Survival::GetSettings(a_feature);
		if (!settings)
		{
			a_vm->TraceStack("aiFeature is not a valid Survival feature", a_stackID, Severity::kWarning);
			return;
		}

		settings->UserDisable();
	}

	void UserReset(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, Feature a_feature)
	{
		auto settings = Survival::GetSettings(a_feature);
		if (!settings)
		{
			a_vm->TraceStack("aiFeature is not a valid Survival feature", a_stackID, Severity::kWarning);
			return;
		}

		settings->UserReset();
	}

	bool RegisterFuncs(VM* a_vm)
	{
		a_vm->RegisterFunction("ForceEnable", "Survival", ForceEnable, true);
		a_vm->RegisterFunction("RequestDisable", "Survival", RequestDisable, true);
		a_vm->RegisterFunction("ModReset", "Survival", ModReset, true);
		a_vm->RegisterFunction("UserEnable", "Survival", UserEnable, true);
		a_vm->RegisterFunction("UserDisable", "Survival", UserDisable, true);
		a_vm->RegisterFunction("UserReset", "Survival", UserReset, true);

		return true;
	}
}