#pragma once

#include "Survival.h"

namespace PapyrusSurvival
{
	using VM = RE::BSScript::IVirtualMachine;
	using StackID = RE::VMStackID;
	using Severity = RE::BSScript::ErrorLogger::Severity;
	using Feature = Survival::Feature;

	int GetVersion(RE::StaticFunctionTag*);
	int GetVersionMinor(RE::StaticFunctionTag*);

	void ForceEnable(RE::StaticFunctionTag*, Feature a_feature);
	void RequestDisable(RE::StaticFunctionTag*, Feature a_feature);

	void ModReset(RE::StaticFunctionTag*, Feature a_feature);
	void UserEnable(RE::StaticFunctionTag*, Feature a_feature);
	void UserDisable(RE::StaticFunctionTag*, Feature a_feature);
	void UserReset(RE::StaticFunctionTag*, Feature a_feature);

	bool IsEnabledByMods(RE::StaticFunctionTag*, Feature a_feature);
	bool IsDisabledByMods(RE::StaticFunctionTag*, Feature a_feature);
	bool IsEnabledByUser(RE::StaticFunctionTag*, Feature a_feature);
	bool IsDisabledByUser(RE::StaticFunctionTag*, Feature a_feature);

	void EnableFrostfallKeywords(RE::StaticFunctionTag*);
	void DisableFrostfallKeywords(RE::StaticFunctionTag*);
	bool AreFrostfallKeywordsEnabled(RE::StaticFunctionTag*);

	void EnableCloakWarmth(RE::StaticFunctionTag*);
	void DisableCloakWarmth(RE::StaticFunctionTag*);
	bool IsCloakWarmthEnabled(RE::StaticFunctionTag*);

	void SetCloakNormalBonus(RE::StaticFunctionTag*, float a_warmth);
	void SetCloakWarmBonus(RE::StaticFunctionTag*, float a_warmth);
	void SetCloakColdBonus(RE::StaticFunctionTag*, float a_warmth);

	float GetCloakNormalBonus(RE::StaticFunctionTag*);
	float GetCloakWarmBonus(RE::StaticFunctionTag*);
	float GetCloakColdBonus(RE::StaticFunctionTag*);

	void SetArmorWarmthNormal(RE::StaticFunctionTag*, RE::TESObjectARMO* a_armor);
	void SetArmorWarmthWarm(RE::StaticFunctionTag*, RE::TESObjectARMO* a_armor);
	void SetArmorWarmthCold(RE::StaticFunctionTag*, RE::TESObjectARMO* a_armor);
	void ResetArmorWarmthToDefault(RE::StaticFunctionTag*, RE::TESObjectARMO* a_armor);
	void ResetAllArmorWarmthToDefault(RE::StaticFunctionTag*);

	bool RegisterFuncs(VM* a_vm);
}
