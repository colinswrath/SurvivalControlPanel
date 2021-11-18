#pragma once

#include "Offsets.h"
#include "Survival.h"

class SleepLevelUpManager
{
public:
	inline static void Install()
	{
		//REL::Relocation<std::uintptr_t> Disable_World_Level_Hook{ PlayerCharacter_sub_offset, 0x40 };
		//REL::Relocation<std::uintptr_t> Enable_SleepSkills_Menu_Hook{ StatsMenu_Sub_offset, 0xF2C };
		//REL::Relocation<std::uintptr_t> Tween_Menu_Hook{ TweenMenu_Sub_offset, 0x7B };

		REL::Relocation<std::uintptr_t> SleepToLevelUp_Hook{ Offset::PlayerCharacter::StopSleepWait.address() + 0x40 };
		REL::Relocation<std::uintptr_t> StatsMenu_Hook{ Offset::StatsMenu::ProcessMessage.address() + 0xFB0 };
		REL::Relocation<std::uintptr_t> TweenMenu_Hook{ Offset::TweenMenu::OpenMenu.address() + 0x7B };

		auto& trampoline = SKSE::GetTrampoline();
		trampoline.write_call<5>(SleepToLevelUp_Hook.address(), IsSleepToLevelUpEnabled);

		trampoline.write_call<5>(StatsMenu_Hook.address(), IsSleepToLevelUpEnabled);

		trampoline.write_call<5>(TweenMenu_Hook.address(), IsSleepToLevelUpEnabled);

		logger::info("Installed hook for sleep to level up."sv);
	}

private:
	inline static bool IsSleepToLevelUpEnabled()
	{
		return Survival::FeatureIsEnabled(Survival::Feature::SleepToLevelUp);
	}
};
