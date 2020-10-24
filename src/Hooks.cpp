#include "Hooks.h"
#include "Offsets.h"

namespace Hooks
{
	// TODO: move to own file
	class SurvivalHookManager
	{
	public:
		inline static void Install()
		{
			REL::Relocation<std::uintptr_t> GetWeight_ArrowWeight_Hook{ TESBoundObject_GetWeight_offset, 0x7C };

			auto& trampoline = SKSE::GetTrampoline();
			_IsArrowWeightEnabled = trampoline.write_call<5>(GetWeight_ArrowWeight_Hook.address(), IsArrowWeightEnabled);

			logger::info("Installed hook for arrow weight"sv);
		}

	private:
		inline static bool IsArrowWeightEnabled()
		{
			return true;
			//return _IsArrowWeightEnabled();
		}

		inline static REL::Relocation<decltype(IsArrowWeightEnabled)> _IsArrowWeightEnabled;
	};

	void Install()
	{
		SurvivalHookManager::Install();

		logger::info("Installed all hooks"sv);
	}
}