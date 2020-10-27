#include "Papyrus.h"
#include "PapyrusSurvival.h"
#include "JContainers.h"

namespace Papyrus
{
	void Register()
	{
		auto papyrus = SKSE::GetPapyrusInterface();

		papyrus->Register(PapyrusSurvival::RegisterFuncs);
		papyrus->Register(JContainers::RegisterFuncs);
		logger::info("Registered Survival script");
	}
}