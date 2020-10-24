#include "Hooks.h"
#include "ArrowWeightManager.h"

namespace Hooks
{
	void Install()
	{
		ArrowWeightManager::Install();

		logger::info("Installed all hooks"sv);
	}
}