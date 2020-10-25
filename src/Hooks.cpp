#include "Hooks.h"
#include "ArrowWeightManager.h"
#include "LockpickWeightManager.h"
#include "SleepLevelUpManager.h"
#include "WarmthManager.h"

namespace Hooks
{
	void Install()
	{
		ArrowWeightManager::Install();
		LockpickWeightManager::Install();
		SleepLevelUpManager::Install();
		WarmthManager::Install();

		logger::info("Installed all hooks"sv);
	}
}