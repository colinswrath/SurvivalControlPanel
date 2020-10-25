#include "Hooks.h"
#include "ArrowWeightManager.h"
#include "LockpickWeightManager.h"
#include "SleepLevelUpManager.h"
#include "WarmthInventoryUIManager.h"

namespace Hooks
{
	void Install()
	{
		ArrowWeightManager::Install();
		LockpickWeightManager::Install();
		SleepLevelUpManager::Install();
		WarmthInventoryUIManager::Install();

		logger::info("Installed all hooks"sv);
	}
}