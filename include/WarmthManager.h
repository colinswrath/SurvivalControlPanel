#pragma once

#include "Offsets.h"
#include "Warmth.h"

class WarmthManager
{
public:
	using WarmthClass = Survival::WarmthClass;

	inline static void Install()
	{
		REL::Relocation<std::uintptr_t> GetWarmthRating_Hook{ Offset::TESBoundObject::GetWarmthRating, 0x6C };

		auto& trampoline = SKSE::GetTrampoline();
		_GetWarmthInfo = trampoline.write_call<5>(GetWarmthRating_Hook.address(), GetWarmthInfo);

		logger::info("Installed hook for armor warmth"sv);
	}

	inline static WarmthClass GetWarmthClass(RE::TESObjectARMO* armor)
	{
		std::int32_t slotMask{};
		RE::Setting** warmthValues{};

		ArmorWarmthInfo info{};
		info.slotMask = &slotMask;
		info.warmthValues = &warmthValues;

		GetWarmthInfo(armor, &info);
		return GetWarmthClass(&info);
	}

private:
	struct ArmorWarmthInfo
	{
		void* _pad_0;
		std::int32_t* slotMask;
		RE::Setting*** warmthValues;
	};
	static_assert(sizeof(ArmorWarmthInfo) == 0x18);

	inline static void GetWarmthInfo(RE::BGSKeywordForm* form, ArmorWarmthInfo* info)
	{
		_GetWarmthInfo(form, info);
		auto armorForm = skyrim_cast<RE::TESObjectARMO*>(form);
		if (!armorForm) {
			return;
		}

		auto warmthClass = GetWarmthClass(info);
		auto& warmthSettings = Survival::WarmthSettings::GetSingleton();

		constexpr RE::FormID FrostfallEnableKeywordProtection{ 0x01CC0E28 };
		static auto kywdFrostfallEnable =
			RE::TESForm::LookupByID(FrostfallEnableKeywordProtection)->As<RE::BGSKeyword>();

		if (warmthSettings.EnableFrostfallKeywords && kywdFrostfallEnable) {

			if (form->HasKeyword(kywdFrostfallEnable)) {
				for (std::uint32_t i = 0; i < form->numKeywords; i++) {
					auto keyword = form->keywords[i];

					constexpr RE::FormID FrostfallWarmthPoor{ 0x01CC0E0F };
					constexpr RE::FormID FrostfallWarmthFair{ 0x01CC0E11 };
					constexpr RE::FormID FrostfallWarmthGood{ 0x01CC0E12 };
					constexpr RE::FormID FrostfallWarmthExcellent{ 0x01CC0E13 };
					constexpr RE::FormID FrostfallWarmthMax{ 0x01CC0E14 };
					constexpr RE::FormID FrostfallIsCloakFur{ 0x01CC0E1E };

					switch (keyword->formID) {
					case FrostfallWarmthPoor:
						*info->warmthValues = GetWarmthSettings(WarmthClass::Cold);
						break;
					case FrostfallWarmthFair:
					case FrostfallWarmthGood:
						*info->warmthValues = GetWarmthSettings(WarmthClass::Normal);
						break;
					case FrostfallWarmthExcellent:
					case FrostfallWarmthMax:
						*info->warmthValues = GetWarmthSettings(WarmthClass::Warm);
						break;
					}
				}
			}
		}

		auto it = warmthSettings.WarmthOverrides.find(armorForm->formID);
		if (it != warmthSettings.WarmthOverrides.end()) {
			warmthClass = it->second;
			*info->warmthValues = GetWarmthSettings(warmthClass);
		}

		if (warmthSettings.EnableWarmthForCloaks) {
			auto kCloak = static_cast<RE::BIPED_MODEL::BipedObjectSlot>(1 << 16);
			bool isCloak = armorForm->bipedModelData.bipedObjectSlots.any(kCloak);

			if (isCloak) {
				*info->warmthValues = warmthSettings.GetCloakSettings(warmthClass);
				*info->slotMask = 0x1;
			}
		}
	}

	inline static WarmthClass GetWarmthClass(ArmorWarmthInfo* info)
	{
		auto& warmthSettings = Survival::WarmthSettings::GetSingleton();

		if (*info->warmthValues == GetWarmthSettings(WarmthClass::Warm) ||
			*info->warmthValues == warmthSettings.GetCloakSettings(WarmthClass::Warm)) {
			return WarmthClass::Warm;
		} else if (*info->warmthValues == GetWarmthSettings(WarmthClass::Cold) ||
				   *info->warmthValues == warmthSettings.GetCloakSettings(WarmthClass::Cold)) {
			return WarmthClass::Cold;
		} else {
			return WarmthClass::Normal;
		}
	}

	inline static RE::Setting** GetWarmthSettings(WarmthClass warmthClass)
	{
		static REL::Relocation<RE::Setting**> normal{ Offset::Survival::WarmthSettings_Normal };
		static REL::Relocation<RE::Setting**> warm{ Offset::Survival::WarmthSettings_Warm };
		static REL::Relocation<RE::Setting**> cold{ Offset::Survival::WarmthSettings_Cold };

		switch (warmthClass) {
		case WarmthClass::Warm:
			return warm.get();
		case WarmthClass::Cold:
			return cold.get();
		default:
			return normal.get();
		}
	}

	inline static REL::Relocation<decltype(GetWarmthInfo)> _GetWarmthInfo;
};
