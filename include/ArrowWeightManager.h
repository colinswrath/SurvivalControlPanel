#pragma once

#include "Offsets.h"
#include "Survival.h"

class ArrowWeightManager
{
public:
	inline static void Install()
	{
		REL::Relocation<std::uintptr_t> GetWeight_ArrowWeight_Hook{ Offset::TESBoundObject::GetWeight, 0x6E };

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
