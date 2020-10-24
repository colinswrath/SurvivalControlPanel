#include "Hooks.h"
#include "ArrowWeightManager.h"
#include "LockpickWeightManager.h"

namespace Hooks
{
	void Install()
	{
		ArrowWeightManager::Install();
		LockpickWeightManager::Install();

		logger::info("Installed all hooks"sv);
	}
}