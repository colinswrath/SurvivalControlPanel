#include "Papyrus.h"
#include "PapyrusJson.h"
#include "PapyrusSurvival.h"

namespace Papyrus
{
	void Register()
	{
		auto papyrus = SKSE::GetPapyrusInterface();

		papyrus->Register(PapyrusSurvival::RegisterFuncs);
		papyrus->Register(PapyrusJson::RegisterFuncs);
		logger::info("Registered Survival script"sv);
	}
}
