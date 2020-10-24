#include "Papyrus.h"
#include "PapyrusSurvival.h"

namespace Papyrus
{
	void Register()
	{
		auto papyrus = SKSE::GetPapyrusInterface();

		papyrus->Register(PapyrusSurvival::RegisterFuncs);
		logger::info("Registered Survival script");
	}
}