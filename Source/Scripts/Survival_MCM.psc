Scriptname Survival_MCM extends SKI_ConfigBase

import Game
import Survival

int Property DEFAULT_INDEX  = 0  AutoReadOnly
int Property DISABLED_INDEX = 1  AutoReadOnly
int Property ENABLED_INDEX  = 2  AutoReadOnly

float Property WARMTH_MIN = 0.0  AutoReadOnly
float Property WARMTH_MAX = 60.0  AutoReadOnly

String[] MenuEntries
int[] MenuOptions
int FrostfallToggle
int CloakWarmthToggle

int BodyNormalSlider
int BodyWarmSlider
int BodyColdSlider
int HeadNormalSlider
int HeadWarmSlider
int HeadColdSlider
int HandsNormalSlider
int HandsWarmSlider
int HandsColdSlider
int FeetNormalSlider
int FeetWarmSlider
int FeetColdSlider
int CloakNormalSlider
int CloakWarmSlider
int CloakColdSlider
int TorchSlider
int ScalarSlider

Event OnInit()
	parent.OnInit()

	MenuEntries = new String[3]
	MenuEntries[DEFAULT_INDEX] = "$Default"
	MenuEntries[DISABLED_INDEX] = "$Always Disabled"
	MenuEntries[ENABLED_INDEX] = "$Always Enabled"

	MenuOptions = new int[5]
EndEvent

Event OnPageReset(String a_page)
	if a_page == "$Survival Mode Core"
		SetCursorFillMode(TOP_TO_BOTTOM)

		AddHeaderOption("$Survival Mode Features")

		AddToggleOptions("$HUD indicators", 0)
		AddToggleOptions("$Inventory UI", 1)
		AddToggleOptions("$Sleep to level up", 2)
		AddToggleOptions("$Arrow weight", 3)
		AddToggleOptions("$Lockpick weight", 4)

		SetCursorPosition(1)

		AddHeaderOption("$Survival Mode Patches")

		FrostfallToggle = AddToggleOption("$Enable Frostfall Keywords", false)
		CloakWarmthToggle = AddToggleOption("$Enable Cloak Warmth", IsCloakWarmthEnabled())
	elseif a_page == "$Warmth Options"
		SetCursorFillMode(TOP_TO_BOTTOM)

		AddHeaderOption("$Body")
		BodyNormalSlider = AddSliderOption("$Body Normal Bonus", GetGameSettingFloat("fSurvNormalBodyBonus"))
		BodyWarmSlider = AddSliderOption("$Body Warm Bonus", GetGameSettingFloat("fSurvWarmBodyBonus"))
		BodyColdSlider = AddSliderOption("$Body Cold Bonus", GetGameSettingFloat("fSurvColdBodyBonus"))

		AddHeaderOption("$Hands")
		HandsNormalSlider = AddSliderOption("$Hands Normal Bonus", GetGameSettingFloat("fSurvNormalHandsBonus"))
		HandsWarmSlider = AddSliderOption("$Hands Warm Bonus", GetGameSettingFloat("fSurvWarmHandsBonus"))
		HandsColdSlider = AddSliderOption("$Hands Cold Bonus", GetGameSettingFloat("fSurvColdHandsBonus"))

		AddHeaderOption("$Cloak")
		int iFlag = OPTION_FLAG_NONE
		if (!IsCloakWarmthEnabled())
			iFlag = OPTION_FLAG_DISABLED
		endif
		CloakNormalSlider = AddSliderOption("$Cloak Normal Bonus", GetCloakNormalBonus(), a_flags = iFlag)
		CloakWarmSlider = AddSliderOption("$Cloak Warm Bonus", GetCloakWarmBonus(), a_flags = iFlag)
		CloakColdSlider = AddSliderOption("$Cloak Cold Bonus", GetCloakColdBonus(), a_flags = iFlag)

		SetCursorPosition(1)

		AddHeaderOption("$Head")
		HeadNormalSlider = AddSliderOption("$Head Normal Bonus", GetGameSettingFloat("fSurvNormalHeadBonus"))
		HeadWarmSlider = AddSliderOption("$Head Warm Bonus", GetGameSettingFloat("fSurvWarmHeadBonus"))
		HeadColdSlider = AddSliderOption("$Head Cold Bonus", GetGameSettingFloat("fSurvColdHeadBonus"))

		AddHeaderOption("$Feet")
		FeetNormalSlider = AddSliderOption("$Feet Normal Bonus", GetGameSettingFloat("fSurvNormalFeetBonus"))
		FeetWarmSlider = AddSliderOption("$Feet Warm Bonus", GetGameSettingFloat("fSurvWarmFeetBonus"))
		FeetColdSlider = AddSliderOption("$Feet Cold Bonus", GetGameSettingFloat("fSurvColdFeetBonus"))

		AddHeaderOption("$Other")
		ScalarSlider = AddSliderOption("$Scaling Factor", GetGameSettingFloat("fSurvArmorScalar"), "{2}")
		TorchSlider = AddSliderOption("$Torch Bonus", GetGameSettingFloat("fSurvTorchBonus"))
	endif
EndEvent

Function AddToggleOptions(String a_name, int a_feature)

	bool bUserEnabled = IsEnabledByUser(a_feature)
	bool bUserDisabled = IsDisabledByUser(a_feature)
	bool bModEnabled = IsEnabledByMods(a_feature)
	bool bModDisabled = IsDisabledByMods(a_feature)

	if bUserEnabled
		MenuOptions[a_feature] = AddMenuOption(a_name, "$Always Enabled")
	elseif bUserDisabled
		MenuOptions[a_feature] = AddMenuOption(a_name, "$Always Disabled")
	else
		MenuOptions[a_feature] = AddMenuOption(a_name, "$Default")
	endif
EndFunction

Event OnOptionMenuOpen(int a_option)
	int iFeature = 0
	while a_option != MenuOptions[iFeature] && iFeature < 5
		iFeature += 1
	endwhile

	SetMenuDialogOptions(MenuEntries)
	SetMenuDialogDefaultIndex(DEFAULT_INDEX)

	if IsEnabledByUser(iFeature)
		SetMenuDialogStartIndex(ENABLED_INDEX)
	elseif IsDisabledByUser(iFeature)
		SetMenuDialogStartIndex(DISABLED_INDEX)
	else
		SetMenuDialogStartIndex(DEFAULT_INDEX)
	endif
EndEvent

Event OnOptionMenuAccept(int a_option, int index)
	int iFeature = 0
	while a_option != MenuOptions[iFeature] && iFeature < 5
		iFeature += 1
	endwhile

	if index == DEFAULT_INDEX
		UserReset(iFeature)
		SetMenuOptionValue(a_option, MenuEntries[DEFAULT_INDEX])
	elseif index == DISABLED_INDEX
		UserDisable(iFeature)
		SetMenuOptionValue(a_option, MenuEntries[DISABLED_INDEX])
	elseif index == ENABLED_INDEX
		UserEnable(iFeature)
		SetMenuOptionValue(a_option, MenuEntries[ENABLED_INDEX])
	endif
EndEvent

Event OnOptionSelect(int a_option)
	if a_option == CloakWarmthToggle
		if IsCloakWarmthEnabled()
			DisableCloakWarmth()
			SetToggleOptionValue(CloakWarmthToggle, false)
		else
			EnableCloakWarmth()
			SetToggleOptionValue(CloakWarmthToggle, true)
		endif
	endif
EndEvent

Event OnOptionSliderOpen(int a_option)
	if a_option == BodyNormalSlider
		SetSliderDialogStartValue(GetGameSettingFloat("fSurvNormalBodyBonus"))
		SetSliderDialogDefaultValue(27.0)
	elseif a_option == BodyWarmSlider
		SetSliderDialogStartValue(GetGameSettingFloat("fSurvWarmBodyBonus"))
		SetSliderDialogDefaultValue(54.0)
		SetSliderDialogRange(WARMTH_MIN, WARMTH_MAX)
		SetSliderDialogInterval(1.0)
	elseif a_option == BodyColdSlider
		SetSliderDialogStartValue(GetGameSettingFloat("fSurvColdBodyBonus"))
		SetSliderDialogDefaultValue(17.0)
		SetSliderDialogRange(WARMTH_MIN, WARMTH_MAX)
		SetSliderDialogInterval(1.0)
	elseif a_option == HeadNormalSlider
		SetSliderDialogStartValue(GetGameSettingFloat("fSurvNormalHeadBonus"))
		SetSliderDialogDefaultValue(18.0)
		SetSliderDialogRange(WARMTH_MIN, WARMTH_MAX)
		SetSliderDialogInterval(1.0)
	elseif a_option == HeadWarmSlider
		SetSliderDialogStartValue(GetGameSettingFloat("fSurvWarmHeadBonus"))
		SetSliderDialogDefaultValue(29.0)
		SetSliderDialogRange(WARMTH_MIN, WARMTH_MAX)
		SetSliderDialogInterval(1.0)
	elseif a_option == HeadColdSlider
		SetSliderDialogStartValue(GetGameSettingFloat("fSurvColdHeadBonus"))
		SetSliderDialogDefaultValue(8.0)
		SetSliderDialogRange(WARMTH_MIN, WARMTH_MAX)
		SetSliderDialogInterval(1.0)
	elseif a_option == HandsNormalSlider
		SetSliderDialogStartValue(GetGameSettingFloat("fSurvNormalHandsBonus"))
		SetSliderDialogDefaultValue(13.0)
		SetSliderDialogRange(WARMTH_MIN, WARMTH_MAX)
		SetSliderDialogInterval(1.0)
	elseif a_option == HandsWarmSlider
		SetSliderDialogStartValue(GetGameSettingFloat("fSurvWarmHandsBonus"))
		SetSliderDialogDefaultValue(24.0)
		SetSliderDialogRange(WARMTH_MIN, WARMTH_MAX)
		SetSliderDialogInterval(1.0)
	elseif a_option == HandsColdSlider
		SetSliderDialogStartValue(GetGameSettingFloat("fSurvColdHandsBonus"))
		SetSliderDialogDefaultValue(7.0)
		SetSliderDialogRange(WARMTH_MIN, WARMTH_MAX)
		SetSliderDialogInterval(1.0)
	elseif a_option == FeetNormalSlider
		SetSliderDialogStartValue(GetGameSettingFloat("fSurvNormalFeetBonus"))
		SetSliderDialogDefaultValue(13.0)
		SetSliderDialogRange(WARMTH_MIN, WARMTH_MAX)
		SetSliderDialogInterval(1.0)
	elseif a_option == FeetWarmSlider
		SetSliderDialogStartValue(GetGameSettingFloat("fSurvWarmFeetBonus"))
		SetSliderDialogDefaultValue(24.0)
		SetSliderDialogRange(WARMTH_MIN, WARMTH_MAX)
		SetSliderDialogInterval(1.0)
	elseif a_option == FeetColdSlider
		SetSliderDialogStartValue(GetGameSettingFloat("fSurvColdFeetBonus"))
		SetSliderDialogDefaultValue(7.0)
		SetSliderDialogRange(WARMTH_MIN, WARMTH_MAX)
		SetSliderDialogInterval(1.0)
	elseif a_option == CloakNormalSlider
		SetSliderDialogStartValue(GetCloakNormalBonus())
		SetSliderDialogDefaultValue(18.0)
		SetSliderDialogRange(WARMTH_MIN, WARMTH_MAX)
		SetSliderDialogInterval(1.0)
	elseif a_option == CloakWarmSlider
		SetSliderDialogStartValue(GetCloakWarmBonus())
		SetSliderDialogDefaultValue(29.0)
		SetSliderDialogRange(WARMTH_MIN, WARMTH_MAX)
		SetSliderDialogInterval(1.0)
	elseif a_option == CloakColdSlider
		SetSliderDialogStartValue(GetCloakColdBonus())
		SetSliderDialogDefaultValue(8.0)
		SetSliderDialogRange(WARMTH_MIN, WARMTH_MAX)
		SetSliderDialogInterval(1.0)
	elseif a_option == TorchSlider
		SetSliderDialogStartValue(GetGameSettingFloat("fSurvTorchBonus"))
		SetSliderDialogDefaultValue(50.0)
		SetSliderDialogRange(WARMTH_MIN, WARMTH_MAX)
		SetSliderDialogInterval(1.0)
	elseif a_option == ScalarSlider
		SetSliderDialogStartValue(GetGameSettingFloat("fSurvArmorScalar"))
		SetSliderDialogDefaultValue(1.0)
		SetSliderDialogRange(0.0, 2.0)
		SetSliderDialogInterval(0.01)
	endif
EndEvent

Event OnOptionSliderAccept(int a_option, float a_value)
	if a_option == BodyNormalSlider
		SetGameSettingFloat("fSurvNormalBodyBonus", a_value)
		SetSliderOptionValue(BodyNormalSlider, a_value)
	elseif a_option == BodyWarmSlider
		SetGameSettingFloat("fSurvWarmBodyBonus", a_value)
		SetSliderOptionValue(BodyWarmSlider, a_value)
	elseif a_option == BodyColdSlider
		SetGameSettingFloat("fSurvColdBodyBonus", a_value)
		SetSliderOptionValue(BodyColdSlider, a_value)
	elseif a_option == HeadNormalSlider
		SetGameSettingFloat("fSurvNormalHeadBonus", a_value)
		SetSliderOptionValue(HeadNormalSlider, a_value)
	elseif a_option == HeadWarmSlider
		SetGameSettingFloat("fSurvWarmHeadBonus", a_value)
		SetSliderOptionValue(HeadWarmSlider, a_value)
	elseif a_option == HeadColdSlider
		SetGameSettingFloat("fSurvColdHeadBonus", a_value)
		SetSliderOptionValue(HeadColdSlider, a_value)
	elseif a_option == HandsNormalSlider
		SetGameSettingFloat("fSurvNormalHandsBonus", a_value)
		SetSliderOptionValue(HandsNormalSlider, a_value)
	elseif a_option == HandsWarmSlider
		SetGameSettingFloat("fSurvWarmHandsBonus", a_value)
		SetSliderOptionValue(HandsWarmSlider, a_value)
	elseif a_option == HandsColdSlider
		SetGameSettingFloat("fSurvColdHandsBonus", a_value)
		SetSliderOptionValue(HandsColdSlider, a_value)
	elseif a_option == FeetNormalSlider
		SetGameSettingFloat("fSurvNormalFeetBonus", a_value)
		SetSliderOptionValue(FeetNormalSlider, a_value)
	elseif a_option == FeetWarmSlider
		SetGameSettingFloat("fSurvWarmFeetBonus", a_value)
		SetSliderOptionValue(FeetWarmSlider, a_value)
	elseif a_option == FeetColdSlider
		SetGameSettingFloat("fSurvColdFeetBonus", a_value)
		SetSliderOptionValue(FeetColdSlider, a_value)
	elseif a_option == CloakNormalSlider
		SetCloakNormalBonus(a_value)
		SetSliderOptionValue(CloakNormalSlider, a_value)
	elseif a_option == CloakWarmSlider
		SetCloakWarmBonus(a_value)
		SetSliderOptionValue(CloakWarmSlider, a_value)
	elseif a_option == CloakColdSlider
		SetCloakColdBonus(a_value)
		SetSliderOptionValue(CloakColdSlider, a_value)
	elseif a_option == TorchSlider
		SetGameSettingFloat("fSurvTorchBonus", a_value)
		SetSliderOptionValue(TorchSlider, a_value)
	elseif a_option == ScalarSlider
		SetGameSettingFloat("fSurvArmorScalar", a_value)
		SetSliderOptionValue(ScalarSlider, a_value)
	endif
EndEvent
