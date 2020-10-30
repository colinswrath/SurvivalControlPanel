#pragma once

#include "Offsets.h"
#include "Survival.h"

class HUDIndicatorManager
{
public:
	inline static void Install()
	{
		REL::Relocation<std::uintptr_t> HUDIndicator_Hook{ HUDMenu_Update_offset, 0xD5D };

		auto& trampoline = SKSE::GetTrampoline();
		_CheckSurvivalHUD = trampoline.write_call<5>(HUDIndicator_Hook.address(), CheckSurvivalHUD);
	}

private:
	inline static int32_t CheckSurvivalHUD(void* menu, bool survivalModeToggle)
	{
		bool isEnabled = Survival::FeatureIsEnabled(Survival::Feature::HUDIndicators);
		return _CheckSurvivalHUD(menu, isEnabled);
	}

	inline static REL::Relocation<decltype(CheckSurvivalHUD)> _CheckSurvivalHUD;
};