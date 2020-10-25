#pragma once

#include "Offsets.h"
#include "Warmth.h"

class WarmthManager
{
public:
	inline static void Install()
	{
		REL::Relocation<std::uintptr_t> GetWarmthRating_Hook{ TESBoundObject_GetWarmthRating_offset, 0x6E };

		auto& trampoline = SKSE::GetTrampoline();
		_GetWarmthInfo = trampoline.write_call<5>(GetWarmthRating_Hook.address(), GetWarmthInfo);

		logger::info("Installed hook for armor warmth"sv);
	}

private:
	struct ArmorWarmthInfo
	{
		void* _pad_0;
		int32_t* slotMask;
		RE::Setting*** warmthValues;
	};
	static_assert(sizeof(ArmorWarmthInfo) == 0x18);

	inline static void GetWarmthInfo(RE::BGSKeywordForm* form, ArmorWarmthInfo* info)
	{
		_GetWarmthInfo(form, info);
		auto armorForm = skyrim_cast<RE::TESObjectARMO*>(form);
		if (!armorForm)
		{
			return;
		}

		auto warmthClass = GetWarmthClass(info);

		auto& warmthSettings = Survival::WarmthSettings::GetSingleton();
		if (warmthSettings.WarmthOverrides.find(armorForm->formID) != warmthSettings.WarmthOverrides.end())
		{
			warmthClass = warmthSettings.WarmthOverrides[armorForm->formID];
		}

		if (warmthSettings.EnableWarmthForCloaks)
		{
			auto kCloak = static_cast<RE::BIPED_MODEL::BipedObjectSlot>(1 << 16);
			bool isCloak = armorForm->bipedModelData.bipedObjectSlots.any(kCloak);

			if (isCloak)
			{
				*info->warmthValues = warmthSettings.GetCloakSetting(warmthClass);
				*info->slotMask = 0x1;
			}
		}
	}

	inline static Survival::WarmthClass GetWarmthClass(ArmorWarmthInfo* info)
	{
		auto settings = RE::GameSettingCollection::GetSingleton();
		if (**info->warmthValues == settings->GetSetting("fSurvWarmBodyBonus"))
		{
			return Survival::WarmthClass::Warm;
		}
		else if (**info->warmthValues == settings->GetSetting("fSurvColdBodyBonus"))
		{
			return Survival::WarmthClass::Cold;
		}
		else
		{
			return Survival::WarmthClass::Normal;
		}
	}

	inline static REL::Relocation<decltype(GetWarmthInfo)> _GetWarmthInfo;
};
