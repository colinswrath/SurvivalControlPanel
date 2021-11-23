#pragma once

#include "Offsets.h"
#include "Survival.h"

class SleepLevelUpManager
{
public:
	inline static void Install()
	{
		REL::Relocation<std::uintptr_t> SleepToLevelUp_Hook{ Offset::PlayerCharacter::StopSleepWait, 0x40 };
		REL::Relocation<std::uintptr_t> StatsMenu_Hook{ Offset::StatsMenu::ProcessMessage, 0xFB0 };
		REL::Relocation<std::uintptr_t> TweenMenu_Hook{ Offset::TweenMenu::OpenMenu, 0x7B };

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
