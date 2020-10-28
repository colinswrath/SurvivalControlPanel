#include "Survival.h"
#include "Serialization.h"

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

	bool Settings::SerializeSave(SKSE::SerializationInterface* a_intfc, uint32_t a_type, uint32_t a_version)
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

		if (!Serialization::Save(a_intfc, prefVector))
		{
			logger::error("Failed to write data!");
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

		if (!Serialization::Load(a_intfc, settings))
		{
			logger::error("Failed to load data!");
		}

		ModPreference = settings[0];
		UserPreference = settings[1];

		return true;
	}
}