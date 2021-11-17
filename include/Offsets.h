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

constexpr REL::Offset IsSurvivalModeEnabled_offset{ 0x090A2A0 };

constexpr REL::Offset TESBoundObject_GetWeight_offset{ 0x01ACFE0 };
constexpr REL::Offset TESBoundObject_GetWarmthRating_offset{ 0x3D4D60 };

constexpr REL::Offset PlayerCharacter_sub_offset{ 0x6C28E0 };  //Prevent level up in world
constexpr REL::Offset StatsMenu_Sub_offset{ 0x900550 };        //Stats menu when sleeping
constexpr REL::Offset TweenMenu_Sub_offset{ 0x8EE960 };        //Widget

constexpr REL::Offset Inventory_offset{ 0x8C0D10 };
constexpr REL::Offset OnEquipped_StatsUpdate_offset{ 0x8DEE20 };
constexpr REL::Offset UIDescription_offset{ 0x8C33D0 };

constexpr REL::Offset HUDMenu_Update_offset{ 0x8AD130 };

constexpr REL::Offset SurvWarmthSettings_Normal_offset{ 0x1E6C470 };
constexpr REL::Offset SurvWarmthSettings_Warm_offset{ 0x1E6C490 };
constexpr REL::Offset SurvWarmthSettings_Cold_offset{ 0x1E6C4B0 };
