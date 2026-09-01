#pragma once

#include "RE/Offset.h"
#include "SCP/Survival.h"

class LockpickWeightManager
{
public:
	inline static void Install()
	{
		REL::Relocation<std::uintptr_t> GetWeight_LockpickWeight_Hook{ STATIC_OFFSET(TESBoundObject::GetWeight) + 0x87 };
		REL::make_pattern<"E8">().match_or_fail(GetWeight_LockpickWeight_Hook.address());

		auto& trampoline = SKSE::GetTrampoline();
		trampoline.write_call<5>(GetWeight_LockpickWeight_Hook.address(), IsLockpickWeightEnabled);

		logger::info("Installed hook for lockpick weight"sv);
	}

private:
	inline static bool IsLockpickWeightEnabled()
	{
		return Survival::FeatureIsEnabled(Survival::Feature::LockpickWeight);
	}
};
