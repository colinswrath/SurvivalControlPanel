#include "Hooks.h"
#include "ArrowWeightManager.h"
#include "LockpickWeightManager.h"
#include "SleepLevelUpManager.h"
#include "InventoryUIManager.h"
#include "WarmthManager.h"

namespace Hooks
{
	void Install()
	{
		ArrowWeightManager::Install();
		LockpickWeightManager::Install();
		SleepLevelUpManager::Install();
		InventoryUIManager::Install();
		WarmthManager::Install();

		logger::info("Installed all hooks"sv);
	}
}