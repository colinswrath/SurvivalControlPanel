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

	inline static Survival::WarmthClass GetWarmthClass(RE::TESObjectARMO* armor)
	{
		int32_t slotMask;
		RE::Setting** warmthValues;

		ArmorWarmthInfo info;
		info.slotMask = &slotMask;
		info.warmthValues = &warmthValues;

		GetWarmthInfo(armor, &info);
		return GetWarmthClass(&info);
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

		if (warmthSettings.EnableFrostfallKeywords)
		{
			auto dataHandler = RE::TESDataHandler::GetSingleton();
			auto frostfallEnableKeywordProtection =
				skyrim_cast<RE::BGSKeyword*>(dataHandler->LookupForm(RE::FormID{ 0x00CC0E28 }, "Update.esm"sv));

			if (frostfallEnableKeywordProtection && form->HasKeyword(frostfallEnableKeywordProtection))
			{
				for (uint32_t i = 0; i < form->numKeywords; i++)
				{
					auto keyword = form->keywords[i];

					RE::FormID frostfallWarmthPoor{ 0x01CC0E0F };
					RE::FormID frostfallWarmthFair{ 0x01CC0E11 };
					RE::FormID frostfallWarmthGood{ 0x01CC0E12 };
					RE::FormID frostfallWarmthExcellent{ 0x01CC0E13 };
					RE::FormID frostfallWarmthMax{ 0x01CC0E14 };
					RE::FormID frostfallIsCloakFur{ 0x01CC0E1E };

					if (keyword->formID == frostfallWarmthPoor)
					{
						warmthClass = Survival::WarmthClass::Cold;
						*info->warmthValues = GetWarmthSettings(Survival::WarmthClass::Cold);
					}
					else if (keyword->formID == frostfallWarmthFair ||
						keyword->formID == frostfallWarmthGood)
					{
						warmthClass = Survival::WarmthClass::Normal;
						*info->warmthValues = GetWarmthSettings(Survival::WarmthClass::Normal);
					}
					else if (keyword->formID == frostfallWarmthExcellent ||
						keyword->formID == frostfallWarmthMax ||
						keyword->formID == frostfallIsCloakFur)
					{
						warmthClass = Survival::WarmthClass::Warm;
						*info->warmthValues = GetWarmthSettings(Survival::WarmthClass::Warm);
					}
				}
			}
		}

		if (warmthSettings.WarmthOverrides.find(armorForm->formID) != warmthSettings.WarmthOverrides.end())
		{
			warmthClass = warmthSettings.WarmthOverrides[armorForm->formID];
			*info->warmthValues = GetWarmthSettings(warmthClass);
		}

		if (warmthSettings.EnableWarmthForCloaks)
		{
			auto kCloak = static_cast<RE::BIPED_MODEL::BipedObjectSlot>(1 << 16);
			bool isCloak = armorForm->bipedModelData.bipedObjectSlots.any(kCloak);

			if (isCloak)
			{
				*info->warmthValues = warmthSettings.GetCloakSettings(warmthClass);
				*info->slotMask = 0x1;
			}
		}
	}

	inline static Survival::WarmthClass GetWarmthClass(ArmorWarmthInfo* info)
	{
		auto& warmthSettings = Survival::WarmthSettings::GetSingleton();
		if (*info->warmthValues == GetWarmthSettings(Survival::WarmthClass::Warm) ||
			*info->warmthValues == warmthSettings.GetCloakSettings(Survival::WarmthClass::Warm))
		{
			return Survival::WarmthClass::Warm;
		}
		else if (*info->warmthValues == GetWarmthSettings(Survival::WarmthClass::Cold) ||
			*info->warmthValues == warmthSettings.GetCloakSettings(Survival::WarmthClass::Cold))
		{
			return Survival::WarmthClass::Cold;
		}
		else
		{
			return Survival::WarmthClass::Normal;
		}
	}

	inline static RE::Setting** GetWarmthSettings(Survival::WarmthClass warmthClass)
	{
		static REL::Relocation<RE::Setting**> normal{ SurvWarmthSettings_Normal_offset.address() };
		static REL::Relocation<RE::Setting**> warm{ SurvWarmthSettings_Warm_offset.address() };
		static REL::Relocation<RE::Setting**> cold{ SurvWarmthSettings_Cold_offset.address() };

		switch (warmthClass) {
		case Survival::WarmthClass::Warm:
			return warm.get();
		case Survival::WarmthClass::Cold:
			return cold.get();
		default:
			return normal.get();
		}
	}

	inline static REL::Relocation<decltype(GetWarmthInfo)> _GetWarmthInfo;
};
