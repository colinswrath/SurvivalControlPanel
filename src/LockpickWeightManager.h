#pragma once

#include "Offsets.h"
#include "Survival.h"

class LockpickWeightManager
{
public:
	inline static void Install()
	{
		REL::Relocation<std::uintptr_t> GetWeight_LockpickWeight_Hook{ TESBoundObject_GetWeight_offset, 0x9E };

		auto& trampoline = SKSE::GetTrampoline();
		LockpickWeightHook = trampoline.write_call<5>(GetWeight_LockpickWeight_Hook.address(), IsLockpickWeightEnabled);

		logger::info("Installed hook for lockpick weight"sv);
	}

private:
	inline static bool IsLockpickWeightEnabled()
	{
		return Survival::IsEnabled(Survival::Feature::LockpickWeight, LockpickWeightHook);
	}

	inline static SurvivalModeCallback LockpickWeightHook;
};
