#pragma once

#include "Offsets.h"
#include "Survival.h"

class InventoryUIManager
{
public:
	inline static void Install()
	{
		REL::Relocation<std::uintptr_t> Show_Warmth_Hook{ Inventory_offset, 0xA82 };
		REL::Relocation<std::uintptr_t> StatsCalcEnable_Hook{ OnEquipped_StatsUpdate_offset, 0x119 };
		REL::Relocation<std::uintptr_t> UIDescription_Hook{ UIDescription_offset, 0x161 };

		auto& trampoline = SKSE::GetTrampoline();
		trampoline.write_call<5>(Show_Warmth_Hook.address(), IsInventoryUIEnabled);

		logger::info("Installed hook for inventory warmth hook."sv);

		trampoline.write_call<5>(StatsCalcEnable_Hook.address(), IsInventoryUIEnabled);

		logger::info("Installed enabling warmth calculation hook."sv);

		trampoline.write_call<5>(UIDescription_Hook.address(), IsInventoryUIEnabled);

		logger::info("Installed hook for UI description."sv);
	}

private:
	inline static bool IsInventoryUIEnabled()
	{
		return Survival::FeatureIsEnabled(Survival::Feature::InventoryUI);
	}
};
