#include "Hooks.h"
#include "ArrowWeightManager.h"
#include "HUDIndicatorManager.h"
#include "InventoryUIManager.h"
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
		InventoryUIManager::Install();
		WarmthManager::Install();
		HUDIndicatorManager::Install();

		logger::info("Installed all hooks"sv);
	}
}
