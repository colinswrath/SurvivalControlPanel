#include "Survival.h"

namespace Survival
{
	void Settings::ForceEnable()
	{
		ModPreference = Preference::AlwaysEnabled;
	}

	void Settings::RequestDisable()
	{
		if (ModPreference == Preference::None)
		{
			ModPreference = Preference::AlwaysDisabled;
		}
	}

	void Settings::ModReset()
	{
		ModPreference = Preference::None;
	}

	void Settings::UserEnable()
	{
		UserPreference = Preference::AlwaysEnabled;
	}

	void Settings::UserDisable()
	{
		UserPreference = Preference::AlwaysDisabled;
	}

	void Settings::UserReset()
	{
		UserPreference = Preference::None;
	}

	bool Settings::IsEnabled()
	{
		if (UserPreference == Preference::AlwaysEnabled)
		{
			return true;
		}
		else if (UserPreference == Preference::AlwaysDisabled)
		{
			return false;
		}
		else if (ModPreference == Preference::AlwaysEnabled)
		{
			return true;
		}
		else if (ModPreference == Preference::AlwaysDisabled)
		{
			return false;
		}
		else
		{
			return ModeIsEnabled();
		}
	}

	bool Settings::SerializeSave(SKSE::SerializationInterface* a_intfc, UINT32 a_type, UINT32 a_version)
	{
		if (!a_intfc->OpenRecord(a_type, a_version))
		{
			logger::error("Failed to open record for Setting!");
			return false;
		}
		else
		{
			return SerializeSave(a_intfc);
		}
	}

	bool Settings::SerializeSave(SKSE::SerializationInterface* a_intfc)
	{
		std::vector<Survival::Preference> prefVector = { ModPreference, UserPreference };

		std::size_t size = prefVector.size();
		if (!a_intfc->WriteRecordData(size))
		{
			logger::error("Failed to write size of record!");
		}
		else
		{
			for (auto& elem : prefVector)
			{
				if (!a_intfc->WriteRecordData(elem))
				{
					logger::error("Failed to write data for elem!");
					break;
				}
			}
		}

		return true;
	}

	/// <summary>
	/// First element in retrieved vector is Mod prefs, second is user
	/// </summary>
	/// <param name="a_intfc"></param>
	/// <returns></returns>
	bool Settings::DeserializeLoad(SKSE::SerializationInterface* a_intfc)
	{
		std::vector<Survival::Preference> settings;

		std::size_t size;
		if (!a_intfc->ReadRecordData(size))
		{
			logger::error("Failed to load size!");
			return false;
		}

		for (UINT32 i = 0; i < size; ++i)
		{
			Survival::Preference elem;
			if (!a_intfc->ReadRecordData(elem))
			{
				logger::error("Failed to load setting element!");
				return false;
			}
			else
			{
				settings.push_back(elem);
			}
		}

		ModPreference = settings[0];
		UserPreference = settings[1];

		return true;
	}
}