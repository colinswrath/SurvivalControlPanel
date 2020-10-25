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
}