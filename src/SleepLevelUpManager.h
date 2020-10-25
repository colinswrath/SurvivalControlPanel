#pragma once

#include "Offsets.h"
#include "Survival.h"

class SleepLevelUpManager
{
public:
	inline static void Install()
	{
		REL::Relocation<std::uintptr_t> Disable_World_Level_Hook{ PlayerCharacter_sub_offset, 0x40 };
		REL::Relocation<std::uintptr_t> Enable_SleepSkills_Menu_Hook{ StatsMenu_Sub_offset, 0xF2C };

		auto& trampoline = SKSE::GetTrampoline();
		DisableWorldLevelingHook = trampoline.write_call<5>(Disable_World_Level_Hook.address(), IsWorldLevelingDisabled);

		logger::info("Installed hook for disabled world leveling."sv);

		SleepToLevelUpHook = trampoline.write_call<5>(Enable_SleepSkills_Menu_Hook.address(), IsSleepToLevelUpEnabled);

		logger::info("Installed hook for sleep to level up."sv);
	}

private:
	inline static bool IsWorldLevelingDisabled()
	{
		return Survival::IsEnabled(Survival::Feature::SleepToLevelUp, DisableWorldLevelingHook);
	}

	inline static bool IsSleepToLevelUpEnabled()
	{
		return Survival::IsEnabled(Survival::Feature::SleepToLevelUp, SleepToLevelUpHook);
	}

	inline static SurvivalModeCallback DisableWorldLevelingHook;
	inline static SurvivalModeCallback SleepToLevelUpHook;

};