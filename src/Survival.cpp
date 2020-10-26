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
		if (!a_intfc->OpenRecord(a_type, 1))
		{
			logger::error("Failed to open record for Setting!");
			return false;
		}
		else
		{
			return SerializeSave(a_intfc);
		}
	}

	//TODO -> remove logging for values
	bool Settings::SerializeSave(SKSE::SerializationInterface* a_intfc)
	{
		std::vector<Survival::Preference> prefVector = { ModPreference, UserPreference };

		for (auto& elem : prefVector)
		{
			if (!a_intfc->WriteRecordData(elem))
			{
				logger::error("Failed to write data for elem!");
				break;
			}
			logger::info(FMT_STRING("Serializing {}"), elem);
		}

		return true;
	}

	bool Settings::DeserializeLoad(SKSE::SerializationInterface* a_intfc)
	{
		return false;
	}
}