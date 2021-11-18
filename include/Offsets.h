#pragma once

//constexpr REL::ID IsSurvivalModeEnabled_offset{ 52058 };
//
//constexpr REL::ID TESBoundObject_GetWeight_offset{ 14809 };
//constexpr REL::ID TESBoundObject_GetWarmthRating_offset{ 25833 };

//constexpr REL::ID PlayerCharacter_sub_offset{ 39346 };	//Prevent level up in world
//constexpr REL::ID StatsMenu_Sub_offset{ 51638 };	//Stats menu when sleeping
//constexpr REL::ID TweenMenu_Sub_offset{ 51843 };	//Widget

//constexpr REL::ID Inventory_offset{ 51019 };
//constexpr REL::ID OnEquipped_StatsUpdate_offset{ 51473 };
//constexpr REL::ID UIDescription_offset{ 51023 };

//constexpr REL::ID HUDMenu_Update_offset{ 50718 };

//constexpr REL::ID SurvWarmthSettings_Normal_offset{ 502630 };
//constexpr REL::ID SurvWarmthSettings_Warm_offset{ 502631 };
//constexpr REL::ID SurvWarmthSettings_Cold_offset{ 502632 };

namespace Offset
{
	namespace BottomBar
	{
		constexpr REL::Offset Update{ 0x8DEE20 };
	}

	namespace HUDMenu
	{
		constexpr REL::Offset Update{ 0x8AD130 };
	}

	namespace ItemMenu
	{
		constexpr REL::Offset ShowItemData{ 0x8C0D10 };
	}

	namespace PlayerCharacter
	{
		constexpr REL::Offset StopSleepWait{ 0x6C28E0 };
	}

	namespace StatsMenu
	{
		constexpr REL::Offset ProcessMessage{ 0x8EE960 };
	}

	namespace Survival
	{
		constexpr REL::Offset IsModeActive{ 0x090A2A0 };
		constexpr REL::Offset ReplaceText{ 0x8C33D0 };
		constexpr REL::Offset WarmthSettings_Normal{ 0x1E6C470 };
		constexpr REL::Offset WarmthSettings_Warm{ 0x1E6C490 };
		constexpr REL::Offset WarmthSettings_Cold{ 0x1E6C4B0 };
	}

	namespace TESBoundObject
	{
		constexpr REL::Offset GetWeight{ 0x01ACFE0 };
		constexpr REL::Offset GetWarmthRating{ 0x3D4D60 };
	}

	namespace TweenMenu
	{
		constexpr REL::Offset OpenMenu{ 0x900550 };
	}
}
