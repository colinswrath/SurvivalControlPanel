#pragma once

#include "RE/Offset.h"
#include "SCP/Survival.h"

class InventoryUIManager
{
public:
	inline static void Install()
	{
		REL::Relocation<std::uintptr_t> ItemData_Hook{ STATIC_OFFSET(ItemMenu::ShowItemData) + 0xC3F };
		REL::Relocation<std::uintptr_t> BottomBar_Hook{ STATIC_OFFSET(BottomBar::Update) + 0x119 };
		REL::Relocation<std::uintptr_t> UIDescription_Hook{ STATIC_OFFSET(Survival::ReplaceText) + 0x276 };

		REL::make_pattern<"E8">().match_or_fail(ItemData_Hook.address());
		REL::make_pattern<"E8">().match_or_fail(BottomBar_Hook.address());
		REL::make_pattern<"E8">().match_or_fail(UIDescription_Hook.address());

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
