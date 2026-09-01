#pragma once

#include "RE/Offset.h"
#include "SCP/Survival.h"

class SleepLevelUpManager
{
public:
	inline static void Install()
	{
		REL::Relocation<std::uintptr_t> SleepToLevelUp_Hook{ STATIC_OFFSET(PlayerCharacter::StopSleepWait) + 0x40 };
		REL::Relocation<std::uintptr_t> StatsMenu_Hook{ STATIC_OFFSET(StatsMenu::ProcessMessage) + 0xFB0 };
		REL::Relocation<std::uintptr_t> TweenMenu_Hook{ STATIC_OFFSET(TweenMenu::OpenMenu) + 0x7B };

		REL::make_pattern<"E8">().match_or_fail(SleepToLevelUp_Hook.address());
		REL::make_pattern<"E8">().match_or_fail(StatsMenu_Hook.address());
		REL::make_pattern<"E8">().match_or_fail(TweenMenu_Hook.address());

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
