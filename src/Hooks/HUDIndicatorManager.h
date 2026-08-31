#pragma once

#include "RE/Offset.h"
#include "SCP/Survival.h"

class HUDIndicatorManager
{
public:
	inline static void Install()
	{
		REL::Relocation<std::uintptr_t> HUDIndicator_Hook{ STATIC_OFFSET(HUDMenu::Update) + 0xD80 };

		auto& trampoline = SKSE::GetTrampoline();
		_CheckSurvivalHUD = trampoline.write_call<5>(HUDIndicator_Hook.address(), CheckSurvivalHUD);

		logger::info("Installed hook for HUD indicator."sv);
	}

private:
	inline static std::int32_t CheckSurvivalHUD(void* menu, bool survivalModeToggle)
	{
		survivalModeToggle = Survival::FeatureIsEnabled(Survival::Feature::HUDIndicators);
		return _CheckSurvivalHUD(menu, survivalModeToggle);
	}

	inline static REL::Relocation<decltype(CheckSurvivalHUD)> _CheckSurvivalHUD;
};
