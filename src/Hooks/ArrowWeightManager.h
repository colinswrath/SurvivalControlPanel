#pragma once

#include "RE/Offset.h"
#include "SCP/Survival.h"

class ArrowWeightManager
{
public:
	inline static void Install()
	{
		REL::Relocation<std::uintptr_t> GetWeight_ArrowWeight_Hook{ STATIC_OFFSET(TESBoundObject::GetWeight) + 0x6E };
		REL::make_pattern<"E8">().match_or_fail(GetWeight_ArrowWeight_Hook.address());

		auto& trampoline = SKSE::GetTrampoline();
		trampoline.write_call<5>(GetWeight_ArrowWeight_Hook.address(), IsArrowWeightEnabled);

		logger::info("Installed hook for arrow weight"sv);
	}

private:
	inline static bool IsArrowWeightEnabled()
	{
		return Survival::FeatureIsEnabled(Survival::Feature::ArrowWeight);
	}
};
