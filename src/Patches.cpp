#include "Patches.h"
#include "xbyak/xbyak.h" 


/// <summary>
/// Includes misc survival patches
/// </summary>
namespace Patches
{	
	namespace stl = SKSE::stl;
	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	bool PatchArrowWeight()
	{
		REL::Relocation<std::uintptr_t> GetWeight_ArrowWeight_Hook(TESBoundObject_GetWeight_offset,0x83);	//0x1A17B3;
		
		/*
		struct Patch : Xbyak::CodeGenerator
		{
			Patch()
			{
				nop();
				nop();
			}
		};	
		
		Patch arrow_patch;
		arrow_patch.ready();
		//Safe write the noop
		REL::safe_write(GetWeight_ArrowWeight_Hook.address(), arrow_patch.getCode(), arrow_patch.getSize());		
		*/
		
		const std::array<UINT8, 2> arrow_patch = { 0x66, 0x90 };
		
		REL::safe_write(GetWeight_ArrowWeight_Hook.address(), arrow_patch);

		return true;
	}
}
