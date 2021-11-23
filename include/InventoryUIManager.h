#pragma once

#include "Offsets.h"
#include "Survival.h"

class InventoryUIManager
{
public:
	inline static void Install()
	{
		REL::Relocation<std::uintptr_t> ItemData_Hook{ Offset::ItemMenu::ShowItemData, 0xC3F };
		REL::Relocation<std::uintptr_t> BottomBar_Hook{ Offset::BottomBar::Update, 0x119 };
		REL::Relocation<std::uintptr_t> UIDescription_Hook{ Offset::Survival::ReplaceText, 0x276 };

		auto& trampoline = SKSE::GetTrampoline();
		trampoline.write_call<5>(ItemData_Hook.address(), IsInventoryUIEnabled);

		logger::info("Installed hook for inventory warmth hook."sv);

		trampoline.write_call<5>(BottomBar_Hook.address(), IsInventoryUIEnabled);

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
