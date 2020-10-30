#pragma once

#include "Offsets.h"
#include "Survival.h"

class HUDIndicatorManager
{
public:
	inline static void Install()
	{
		/*
		REL::Relocation<std::uintptr_t> HUDWidget_Hook{ TweenMenu_Sub_offset, 0x7B };

		auto& trampoline = SKSE::GetTrampoline();
		trampoline.write_call<5>(HUDWidget_Hook.address(), IsHUDIndicatorEnabled);

		logger::info("Installed hook for HUD Indicator."sv);
		*/
	}

private:
	inline static bool IsHUDIndicatorEnabled()
	{
		return Survival::FeatureIsEnabled(Survival::Feature::HUDIndicators);
	}
};