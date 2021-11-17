#include "Papyrus.h"
#include "PapyrusSurvival.h"
#include "PapyrusJson.h"

namespace Papyrus
{
	void Register()
	{
		auto papyrus = SKSE::GetPapyrusInterface();

		papyrus->Register(PapyrusSurvival::RegisterFuncs);
		papyrus->Register(PapyrusJson::RegisterFuncs);
		logger::info("Registered Survival script");
	}
}