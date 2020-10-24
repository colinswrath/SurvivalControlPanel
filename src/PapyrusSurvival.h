#pragma once

#include "Survival.h"

namespace PapyrusSurvival
{
	using VM = RE::BSScript::IVirtualMachine;
	using StackID = RE::VMStackID;
	using Severity = RE::BSScript::ErrorLogger::Severity;
	using Feature = Survival::Feature;

	void ForceEnable(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, Feature a_feature);
	void RequestDisable(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, Feature a_feature);
	void UserEnable(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, Feature a_feature);
	void UserDisable(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, Feature a_feature);
	void UserReset(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, Feature a_feature);

	bool RegisterFuncs(VM* a_vm);
}
