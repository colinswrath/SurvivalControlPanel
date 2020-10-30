#include "Papyrus.h"
#include "PapyrusSurvival.h"
#include "Json.h"

namespace Papyrus
{
	void Register()
	{
		auto papyrus = SKSE::GetPapyrusInterface();

		papyrus->Register(PapyrusSurvival::RegisterFuncs);
		papyrus->Register(Json::RegisterFuncs);
		logger::info("Registered Survival script");
	}
}