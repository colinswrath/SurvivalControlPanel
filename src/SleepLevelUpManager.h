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

		REL::Relocation<std::uintptr_t> Disable_World_Level_Hook{ REL::Offset(PlayerCharacter_sub_offset.offset() + 0x40) };
		REL::Relocation<std::uintptr_t> Enable_SleepSkills_Menu_Hook{ REL::Offset(StatsMenu_Sub_offset.offset() + 0x7B) };
		REL::Relocation<std::uintptr_t> Tween_Menu_Hook{ REL::Offset(TweenMenu_Sub_offset.offset() + 0xFB0) };

		auto& trampoline = SKSE::GetTrampoline();
		trampoline.write_call<5>(Disable_World_Level_Hook.address(), IsSleepToLevelUpEnabled);

		logger::info("Installed hook for disabled world leveling."sv);

		//trampoline.write_call<5>(Enable_SleepSkills_Menu_Hook.address(), IsSleepToLevelUpEnabled);

		//trampoline.write_call<5>(Tween_Menu_Hook.address(), IsSleepToLevelUpEnabled);

		logger::info("Installed hook for sleep to level up."sv);
	}

private:
	inline static bool IsSleepToLevelUpEnabled()
	{
		return Survival::FeatureIsEnabled(Survival::Feature::SleepToLevelUp);
	}
};
