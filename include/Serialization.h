#pragma once

#include "Survival.h"
#include "Version.h"
#include "Warmth.h"

namespace Serialization
{
	enum
	{
		kSerializationVersion = 1,
		kHUDIndicators = 'HUDS',
		kInventoryUI = 'IUIS',
		kSleepToLevelUp = 'STLS',
		kArrowWeight = 'ARWS',
		kLockpickWeight = 'LKPS',
		kWarmthSettings = 'WRMT',
		kHandleId = 'SURV',
	};

	template <typename T>
	bool Save(SKSE::SerializationInterface* a_intfc, std::vector<T> setting)
	{
		std::size_t size = setting.size();
		if (!a_intfc->WriteRecordData(size)) {
			logger::error("Failed to write size of record data!");
			return false;
		} else {
			for (auto& elem : setting) {
				if (!a_intfc->WriteRecordData(elem)) {
					logger::error("Failed to write data for warmth elem!");
					return false;
				}
			}
		}
		return true;
	}

	template <typename T>
	bool Load(SKSE::SerializationInterface* a_intfc, std::vector<T>& returnVector)
	{
		std::size_t size;
		if (!a_intfc->ReadRecordData(size)) {
			logger::error("Failed to load size!");
			return false;
		}

		for (std::size_t i = 0; i < size; ++i) {
			T elem;
			if (!a_intfc->ReadRecordData(elem)) {
				logger::error("Failed to load setting element!");
				return false;
			} else {
				returnVector.push_back(elem);
			}
		}
		return true;
	}

	inline void SaveCallBack(SKSE::SerializationInterface* a_intfc)
	{
		logger::trace("Save callback start"sv);

		auto arrowSettings = Survival::GetSettings(Survival::Feature::ArrowWeight);
		auto uiSettings = Survival::GetSettings(Survival::Feature::InventoryUI);
		auto HUDSettings = Survival::GetSettings(Survival::Feature::HUDIndicators);
		auto lockSettings = Survival::GetSettings(Survival::Feature::LockpickWeight);
		auto SleepSettings = Survival::GetSettings(Survival::Feature::SleepToLevelUp);
		auto& warmthSettings = Survival::WarmthSettings::GetSingleton();

		if (!arrowSettings->SerializeSave(a_intfc, kArrowWeight, kSerializationVersion)) {
			logger::error("Failed to save arrow settings!"sv);
		}

		if (!uiSettings->SerializeSave(a_intfc, kInventoryUI, kSerializationVersion)) {
			logger::error("Failed to save Inventory UI Settings!"sv);
		}

		if (!HUDSettings->SerializeSave(a_intfc, kHUDIndicators, kSerializationVersion)) {
			logger::error("Failed to save Hud Settings Settings!"sv);
		}

		if (!lockSettings->SerializeSave(a_intfc, kLockpickWeight, kSerializationVersion)) {
			logger::error("Failed to save lockpick Settings!"sv);
		}

		if (!SleepSettings->SerializeSave(a_intfc, kSleepToLevelUp, kSerializationVersion)) {
			logger::error("Failed to save Sleep to level Settings!"sv);
		}

		if (!warmthSettings.SerializeSave(a_intfc, kWarmthSettings, kSerializationVersion)) {
			logger::error("Failed to save Warmth Settings!"sv);
		}
		logger::trace("Save callback stop");
	}

	inline void LoadCallBack(SKSE::SerializationInterface* a_intfc)
	{
		logger::trace("load callback start"sv);
		auto arrowSettings = Survival::GetSettings(Survival::Feature::ArrowWeight);
		auto uiSettings = Survival::GetSettings(Survival::Feature::InventoryUI);
		auto HUDSettings = Survival::GetSettings(Survival::Feature::HUDIndicators);
		auto lockSettings = Survival::GetSettings(Survival::Feature::LockpickWeight);
		auto SleepSettings = Survival::GetSettings(Survival::Feature::SleepToLevelUp);
		auto& warmthSettings = Survival::WarmthSettings::GetSingleton();

		std::uint32_t type;
		std::uint32_t version;
		std::uint32_t length;

		while (a_intfc->GetNextRecordInfo(type, version, length)) {
			if (version != kSerializationVersion) {
				logger::error("Loaded data is out of date! Read (%u), expected (%u) for type code (%s)"sv, version, kSerializationVersion, type);
				continue;
			}

			switch (type) {
			case kArrowWeight:
				if (!arrowSettings->DeserializeLoad(a_intfc)) {
					logger::error("Failed to load arrow settings!"sv);
				}
				break;

			case kInventoryUI:

				if (!uiSettings->DeserializeLoad(a_intfc)) {
					logger::error("Failed to load UI settings!"sv);
				}
				break;

			case kHUDIndicators:

				if (!HUDSettings->DeserializeLoad(a_intfc)) {
					logger::error("Failed to load UI settings!"sv);
				}
				break;

			case kLockpickWeight:

				if (!lockSettings->DeserializeLoad(a_intfc)) {
					logger::error("Failed to load UI settings!"sv);
				}
				break;

			case kSleepToLevelUp:

				if (!SleepSettings->DeserializeLoad(a_intfc)) {
					logger::error("Failed to load UI settings!"sv);
				}
				break;
			case kWarmthSettings:

				if (!warmthSettings.DeserializeLoad(a_intfc)) {
					logger::error("Failed to load warmth settings!"sv);
				}
				break;
			default:
				logger::error(FMT_STRING("Unrecognized signature type! {}"), type);
				break;
			}
		}
		logger::trace("load callback stop"sv);
	}
}
