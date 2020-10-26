#include "Warmth.h"
#include "WarmthManager.h"

namespace Survival
{
	float WarmthSettings::GetCloakNormalBonus()
	{
		return _cloakNormalBonus.data;
	}

	void WarmthSettings::SetCloakNormalBonus(float warmth)
	{
		_cloakNormalBonus.data = warmth;
	}

	float WarmthSettings::GetCloakWarmBonus()
	{
		return _cloakWarmBonus.data;
	}

	void WarmthSettings::SetCloakWarmBonus(float warmth)
	{
		_cloakWarmBonus.data = warmth;
	}

	float WarmthSettings::GetCloakColdBonus()
	{
		return _cloakColdBonus.data;
	}

	void WarmthSettings::SetCloakColdBonus(float warmth)
	{
		_cloakColdBonus.data = warmth;
	}

	void WarmthSettings::SetWarmthOverride(RE::TESObjectARMO* armor, WarmthClass warmthClass)
	{
		ResetWarmthOverride(armor);

		if (WarmthManager::GetWarmthClass(armor) != warmthClass)
		{
			WarmthOverrides[armor->formID] = warmthClass;
		}
	}

	void WarmthSettings::ResetWarmthOverride(RE::TESObjectARMO* armor)
	{
		auto it = WarmthOverrides.find(armor->formID);

		if (it != WarmthOverrides.end())
		{
			WarmthOverrides.erase(it);
		}
	}

	RE::Setting** WarmthSettings::GetCloakSettings(WarmthClass warmthClass)
	{
		// Actually, the game expects an array of 4 pointers, but if we set
		// the "slot mask" to 0x1, it will only read one.
		switch (warmthClass) {
		case WarmthClass::Warm:
			return reinterpret_cast<RE::Setting**>(&_cloakWarmBonusPtr);
		case WarmthClass::Cold:
			return reinterpret_cast<RE::Setting**>(&_cloakColdBonusPtr);
		default:
			return reinterpret_cast<RE::Setting**>(&_cloakNormalBonusPtr);
		}
	}
}