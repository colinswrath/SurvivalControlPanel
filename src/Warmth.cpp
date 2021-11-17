#include "Warmth.h"
#include "WarmthManager.h"
#include "Serialization.h"

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

	bool WarmthSettings::SerializeSave(SKSE::SerializationInterface* a_intfc, uint32_t a_type, uint32_t a_version)
	{
		if (!a_intfc->OpenRecord(a_type, a_version))
		{
			logger::error("Failed to open record for Warmth info!");
			return false;
		}
		else
		{
			return SerializeSave(a_intfc);
		}
	}

	//TODO Remove extra logging when no longer needed
	bool WarmthSettings::SerializeSave(SKSE::SerializationInterface* a_intfc)
	{
		std::vector<float> cloakSetting = { GetCloakColdBonus(), GetCloakNormalBonus(), GetCloakWarmBonus() };
		std::array<bool, 2> toggles = { EnableWarmthForCloaks, EnableFrostfallKeywords };

		if (!Serialization::Save(a_intfc, cloakSetting))
		{
			logger::error("Failed to write data!");
		}

		//Write toggle data
		std::size_t size = toggles.size();
		if (!a_intfc->WriteRecordData(size))
		{
			logger::error("Failed to write size of record data!");
		}
		else
		{
			for (auto& elem : toggles)
			{
				if (!a_intfc->WriteRecordData(elem))
				{
					logger::error("Failed to write data for warmth elem!");
					return false;
				}
			}
		}

		//Serialize override table
		const std::size_t mapNums = WarmthOverrides.size();

		if (!a_intfc->WriteRecordData(mapNums))
		{
			logger::error("Failed to write size of record data!");
			return false;
		}

		for (auto& warmth : WarmthOverrides)
		{
			if (!a_intfc->WriteRecordData(warmth.first))
			{
				logger::error("Failed to write warmth elem ({} : {})!",warmth.first, warmth.second);
				return false;
			}

			if (!a_intfc->WriteRecordData(warmth.second))
			{
				logger::error("Failed to write warmth elem ({} : {})!", warmth.first, warmth.second);
				return false;
			}

			logger::info("Wrote warmth elem ({} : {})!", warmth.first, warmth.second);
		}
		return true;
	}

	//TODO Remove extra logging when no longer needed
	bool WarmthSettings::DeserializeLoad(SKSE::SerializationInterface* a_intfc)
	{
		std::vector<float> cloakSetting;
		bool toggles[2];

		//Load cloak info

		if (!Serialization::Load(a_intfc, cloakSetting))
		{
			logger::error("Failed to load data!");
		}

		//Load toggles

		std::size_t size;
		if (!a_intfc->ReadRecordData(size))
		{
			logger::error("Failed to load size!");
			return false;
		}

		for (std::size_t i = 0; i < size; ++i)
		{
			bool elem;
			if (!a_intfc->ReadRecordData(elem))
			{
				logger::error("Failed to load setting element!");
				return false;
			}
			else
			{
				toggles[i] = elem;
			}
		}

		//Load warmth table

		if (!a_intfc->ReadRecordData(size))
		{
			logger::error("Failed to load setting element!");
			return false;
		}

		WarmthOverrides.clear();		//Clean table to get loaded data

		RE::FormID form;
		Survival::WarmthClass warmth;

		for (std::size_t i = 0; i < size; ++i)
		{
			if (!a_intfc->ReadRecordData(form))
			{
				logger::error("Failed to load override form!");
				return false;
			}

			if (!a_intfc->ResolveFormID(form, form))
			{
				logger::error("Failed to resolve formID.");
				return false;
			}

			if (!a_intfc->ReadRecordData(warmth))
			{
				logger::error("Failed to load override warmth setting!");
				return false;
			}

			auto result = WarmthOverrides.insert(std::make_pair(form, warmth));		//load parsed data into table

			if (!result.second)
			{
				logger::error("Failed to insert into table.");
				return false;
			}
		}

		SetCloakColdBonus(cloakSetting[0]);
		SetCloakNormalBonus(cloakSetting[1]);
		SetCloakWarmBonus(cloakSetting[2]);

		EnableWarmthForCloaks = toggles[0];
		EnableFrostfallKeywords = toggles[1];

		return true;
	}
}
