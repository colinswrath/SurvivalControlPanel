Scriptname Survival_MCM extends SKI_ConfigBase

import Game
import Survival

int Property DEFAULT_INDEX  = 0  AutoReadOnly
int Property DISABLED_INDEX = 1  AutoReadOnly
int Property ENABLED_INDEX  = 2  AutoReadOnly

int Property NORMAL_INDEX = 0  AutoReadOnly
int Property WARM_INDEX   = 1  AutoReadOnly
int Property COLD_INDEX   = 2  AutoReadOnly
int Property RESET_INDEX  = 3  AutoReadOnly

float Property WARMTH_MIN = 0.0  AutoReadOnly
float Property WARMTH_MAX = 60.0  AutoReadOnly

int Property BodySlot = 32  AutoReadOnly
int Property HeadSlot = 31  AutoReadOnly
int Property HandsSlot = 33  AutoReadOnly
int Property FeetSlot = 37  AutoReadOnly
int Property CloakSlot = 46  AutoReadOnly

; Survival Mode Core
String[] FeatureMenuEntries
int[] FeatureMenuOptions
int FrostfallToggle
int CloakWarmthToggle

; Warmth Settings
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

; Equipment
String[] WarmthMenuEntries
int BodyOverrideMenu
int HeadOverrideMenu
int HandsOverrideMenu
int FeetOverrideMenu
int CloakOverrideMenu
int ResetOverrideButton

; Profiles
bool CreatedNewFile = false
String CurrentFile = ""

Event OnInit()
	parent.OnInit()

	FeatureMenuEntries = new String[3]
	FeatureMenuEntries[DEFAULT_INDEX] = "$Default"
	FeatureMenuEntries[DISABLED_INDEX] = "$Always Disabled"
	FeatureMenuEntries[ENABLED_INDEX] = "$Always Enabled"

	FeatureMenuOptions = new int[5]

	WarmthMenuEntries = new String[4]
	WarmthMenuEntries[NORMAL_INDEX] = "$NormalWarmth"
	WarmthMenuEntries[WARM_INDEX] = "$Warm"
	WarmthMenuEntries[COLD_INDEX] = "$Cold"
	WarmthMenuEntries[RESET_INDEX] = "$Default"
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

		FrostfallToggle = AddToggleOption("$Enable Frostfall Keywords", AreFrostfallKeywordsEnabled())
		CloakWarmthToggle = AddToggleOption("$Enable Cloak Warmth", IsCloakWarmthEnabled())
	elseif a_page == "$Warmth Settings"
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
	elseif a_page == "$Equipment"
		SetCursorFillMode(TOP_TO_BOTTOM)

		AddHeaderOption("$Current Equipment")

		BodyOverrideMenu = AddEquipmentOptions("$Body", BodySlot, ArmorBody, ClothingBody)
		HeadOverrideMenu = AddEquipmentOptions("$Head", HeadSlot, ArmorHead, ClothingHead)
		HandsOverrideMenu = AddEquipmentOptions("$Hands", HandsSlot, ArmorHands, ClothingHands)
		FeetOverrideMenu = AddEquipmentOptions("$Feet", FeetSlot, ArmorFeet, ClothingFeet)
		if IsCloakWarmthEnabled()
			AddEquipmentOptions("$Cloak", CloakSlot)
		endif

		SetCursorPosition(1)

		AddHeaderOption("$Reset all")
		ResetOverrideButton = AddTextOption("$Reset all to default", "")
	elseif a_page == "$Profiles"
		; This page will use state-based MCM since the options are non-homogenous
		SetCursorFillMode(TOP_TO_BOTTOM)

		int iFlag = OPTION_FLAG_DISABLED
		bool bJCValid = IsJContainersValid()
		if (bJCValid)
			iFlag = OPTION_FLAG_NONE
		endif

		AddHeaderOption("$Profiles")
		AddInputOptionST("NewFile", "$New", "", a_flags = iFlag)
		AddMenuOptionST("Browse", "$Browse", CurrentFile, a_flags = iFlag)

		int iLoadSaveFlag = OPTION_FLAG_DISABLED
		int iDeleteFlag = OPTION_FLAG_DISABLED
		if bJCValid
			if CurrentFile != ""
				iLoadSaveFlag = OPTION_FLAG_NONE
				iDeleteFlag = OPTION_FLAG_NONE
			elseif CreatedNewFile
				iDeleteFlag = OPTION_FLAG_NONE
			endif
		endif

		AddTextOptionST("Load", "$Load", "", a_flags = iLoadSaveFlag)
		AddTextOptionST("Save", "$Save", "", a_flags = iLoadSaveFlag)
		AddTextOptionST("Delete", "$DeleteFile", "", a_flags = iDeleteFlag)
		AddHeaderOption("")

		SetCursorPosition(20)
		AddHeaderOption("JContainers")
		if !JContainers.isInstalled()
			AddTextOption("$API", "$Not installed")
		else
			String sVersion = JContainers.APIVersion()
			sVersion = sVersion + "." + JContainers.featureVersion() + ".x"
			AddTextOption("$API", sVersion)
		endif
	endif
EndEvent

Function AddToggleOptions(String a_name, int a_feature)

	bool bUserEnabled = IsEnabledByUser(a_feature)
	bool bUserDisabled = IsDisabledByUser(a_feature)
	bool bModEnabled = IsEnabledByMods(a_feature)
	bool bModDisabled = IsDisabledByMods(a_feature)

	if bUserEnabled
		FeatureMenuOptions[a_feature] = AddMenuOption(a_name, "$Always Enabled")
	elseif bUserDisabled
		FeatureMenuOptions[a_feature] = AddMenuOption(a_name, "$Always Disabled")
	else
		FeatureMenuOptions[a_feature] = AddMenuOption(a_name, "$Default")
	endif
EndFunction

int Function AddEquipmentOptions(String a_name, int a_slot, Keyword a_keyword1 = None, Keyword a_keyword2 = None)
	int option
	Armor kArmor = GetPlayer().GetEquippedArmorInSlot(a_slot)
	if kArmor && (!a_keyword1 || kArmor.HasKeyword(a_keyword1) || kArmor.HasKeyword(a_keyword2))
		option = AddMenuOption(kArmor.GetName(), GetWarmthRatingAsString(kArmor))
	else
		AddTextOption(a_name, "$NoEquipment", a_flags = OPTION_FLAG_DISABLED)
	endif
	return option
EndFunction

Event OnOptionMenuOpen(int a_option)
	if CurrentPage == "$Survival Mode Core"
		int iFeature = 0
		while a_option != FeatureMenuOptions[iFeature] && iFeature < 5
			iFeature += 1
		endwhile

		SetMenuDialogOptions(FeatureMenuEntries)
		SetMenuDialogDefaultIndex(DEFAULT_INDEX)

		if IsEnabledByUser(iFeature)
			SetMenuDialogStartIndex(ENABLED_INDEX)
		elseif IsDisabledByUser(iFeature)
			SetMenuDialogStartIndex(DISABLED_INDEX)
		else
			SetMenuDialogStartIndex(DEFAULT_INDEX)
		endif
	elseif CurrentPage == "$Equipment"
		int iDefaultIndex = GuessDefaultWarmth(a_option)
		SetMenuDialogOptions(WarmthMenuEntries)
		SetMenuDialogStartIndex(iDefaultIndex)
		SetMenuDialogDefaultIndex(RESET_INDEX)
	endif
EndEvent

Event OnOptionMenuAccept(int a_option, int index)
	if CurrentPage == "$Survival Mode Core"
		int iFeature = 0
		while a_option != FeatureMenuOptions[iFeature] && iFeature < 5
			iFeature += 1
		endwhile

		if index == DEFAULT_INDEX
			UserReset(iFeature)
			SetMenuOptionValue(a_option, FeatureMenuEntries[DEFAULT_INDEX])
		elseif index == DISABLED_INDEX
			UserDisable(iFeature)
			SetMenuOptionValue(a_option, FeatureMenuEntries[DISABLED_INDEX])
		elseif index == ENABLED_INDEX
			UserEnable(iFeature)
			SetMenuOptionValue(a_option, FeatureMenuEntries[ENABLED_INDEX])
		endif
	elseif CurrentPage == "$Equipment"
		Armor kArmor
		if a_option == BodyOverrideMenu
			kArmor = GetPlayer().GetEquippedArmorInSlot(BodySlot)
		elseif a_option == HeadOverrideMenu
			kArmor = GetPlayer().GetEquippedArmorInSlot(HeadSlot)
		elseif a_option == HandsOverrideMenu
			kArmor = GetPlayer().GetEquippedArmorInSlot(HandsSlot)
		elseif a_option == FeetOverrideMenu
			kArmor = GetPlayer().GetEquippedArmorInSlot(FeetSlot)
		else
			return
		endif

		if index == NORMAL_INDEX
			SetArmorWarmthNormal(kArmor)
		elseif index == WARM_INDEX
			SetArmorWarmthWarm(kArmor)
		elseif index == COLD_INDEX
			SetArmorWarmthCold(kArmor)
		elseif index == RESET_INDEX
			ResetArmorWarmthToDefault(kArmor)
		endif

		SetMenuOptionValue(a_option, GetWarmthRatingAsString(kArmor))
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
	elseif a_option == FrostfallToggle
		if AreFrostfallKeywordsEnabled()
			DisableFrostfallKeywords()
			SetToggleOptionValue(FrostfallToggle, false)
		else
			EnableFrostfallKeywords()
			SetToggleOptionValue(FrostfallToggle, true)
		endif
	elseif a_option == ResetOverrideButton
		; TODO Confirmation dialog
		ResetAllArmorWarmthToDefault()
		RecomputeArmorWarmths()
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
		SetSliderOptionValue(ScalarSlider, a_value, "{2}")
	endif
EndEvent

State NewFile
Event OnInputAcceptST(String a_input)
	if (a_input)
		CurrentFile = a_input
		CreatedNewFile = true
		ForcePageReset()
	endif
EndEvent
EndState

State Browse
Event OnMenuOpenST()
	String sProfileDir = JContainersProfileDir()
	if sProfileDir
		SetMenuDialogOptions(JContainers.contentsOfDirectoryAtPath(sProfileDir))
	endif
EndEvent
EndState

State Load
Event OnSelectST()
	; TODO confirmation dialog (This will replace all current settings)
	String sProfileDir = JContainersProfileDir()
	Survival_JContainers.Load(sProfileDir + CurrentFile)
EndEvent
EndState

State Save
Event OnSelectST()
	; TODO confirmation dialog (This will overwrite saved settings)
	String sProfileDir = JContainersProfileDir()
	Survival_JContainers.Save(sProfileDir + CurrentFile)
EndEvent
EndState

State Delete
Event OnSelectST()
	; TODO confirmation dialog (This cannot be undone)
	String sProfileDir = JContainersProfileDir()
	JContainers.removeFileAtPath(sProfileDir + CurrentFile)
	CurrentFile = ""
	CreatedNewFile = false
	ForcePageReset()
EndEvent
EndState

int Function GuessDefaultWarmth(int a_option)
	if a_option == BodyOverrideMenu
		Armor kBody = GetPlayer().GetEquippedArmorInSlot(BodySlot)
		float fWarmth = kBody.GetWarmthRating()
		if fWarmth == GetGameSettingFloat("fSurvNormalBodyBonus")
			return NORMAL_INDEX
		elseif fWarmth == GetGameSettingFloat("fSurvWarmBodyBonus")
			return WARM_INDEX
		elseif fWarmth == GetGameSettingFloat("fSurvColdBodyBonus")
			return COLD_INDEX
		endif
	elseif a_option == HeadOverrideMenu
		Armor kHead = GetPlayer().GetEquippedArmorInSlot(HeadSlot)
		float fWarmth = kHead.GetWarmthRating()
		if fWarmth == GetGameSettingFloat("fSurvNormalHeadBonus")
			return NORMAL_INDEX
		elseif fWarmth == GetGameSettingFloat("fSurvWarmHeadBonus")
			return WARM_INDEX
		elseif fWarmth == GetGameSettingFloat("fSurvColdHeadBonus")
			return COLD_INDEX
		endif
	elseif a_option == HandsOverrideMenu
		Armor kHands = GetPlayer().GetEquippedArmorInSlot(HandsSlot)
		float fWarmth = kHands.GetWarmthRating()
		if fWarmth == GetGameSettingFloat("fSurvNormalHandsBonus")
			return NORMAL_INDEX
		elseif fWarmth == GetGameSettingFloat("fSurvWarmHandsBonus")
			return WARM_INDEX
		elseif fWarmth == GetGameSettingFloat("fSurvColdHandsBonus")
			return COLD_INDEX
		endif
	elseif a_option == FeetOverrideMenu
		Armor kFeet = GetPlayer().GetEquippedArmorInSlot(FeetSlot)
		float fWarmth = kFeet.GetWarmthRating()
		if fWarmth == GetGameSettingFloat("fSurvNormalFeetBonus")
			return NORMAL_INDEX
		elseif fWarmth == GetGameSettingFloat("fSurvWarmFeetBonus")
			return WARM_INDEX
		elseif fWarmth == GetGameSettingFloat("fSurvColdFeetBonus")
			return COLD_INDEX
		endif
	endif

	return RESET_INDEX
EndFunction

Function RecomputeArmorWarmths()
	Armor kBody = GetPlayer().GetEquippedArmorInSlot(BodySlot)
	Armor kHead = GetPlayer().GetEquippedArmorInSlot(HeadSlot)
	Armor kHands = GetPlayer().GetEquippedArmorInSlot(HandsSlot)
	Armor kFeet = GetPlayer().GetEquippedArmorInSlot(FeetSlot)
	Armor kCloak = GetPlayer().GetEquippedArmorInSlot(CloakSlot)

	if kBody && (kBody.HasKeyword(ArmorBody) || kBody.HasKeyword(ClothingBody))
		SetMenuOptionValue(BodyOverrideMenu, GetWarmthRatingAsString(kBody))
	endif
	if kHead && (kHead.HasKeyword(ArmorHead) || kHead.HasKeyword(ClothingHead))
		SetMenuOptionValue(HeadOverrideMenu, GetWarmthRatingAsString(kHead))
	endif
	if kHands && (kHands.HasKeyword(ArmorHands) || kHands.HasKeyword(ClothingHands))
		SetMenuOptionValue(HandsOverrideMenu, GetWarmthRatingAsString(kHands))
	endif
	if kFeet && (kFeet.HasKeyword(ArmorFeet) || kFeet.HasKeyword(ClothingFeet))
		SetMenuOptionValue(FeetOverrideMenu, GetWarmthRatingAsString(kFeet))
	endif
	if IsCloakWarmthEnabled() && kCloak
		SetMenuOptionValue(CloakOverrideMenu, GetWarmthRatingAsString(kCloak))
	endif
EndFunction

String Function GetWarmthRatingAsString(Armor a_armor)
	string sWarmth = a_armor.GetWarmthRating() as string
	sWarmth = StringUtil.Substring(sWarmth, 0, StringUtil.Find(sWarmth, "."))
	return sWarmth
EndFunction

bool Function IsJContainersValid()
	return JContainers.APIVersion() >= 4
EndFunction

String Function JContainersProfileDir()
	String sUserDir = JContainers.userDirectory()
	if sUserDir
		return sUserDir + "Survival/"
	endif
	return ""
EndFunction

Keyword Property ArmorHands
Keyword Function Get()
	return (GetForm(0x31) as DefaultObjectManager).GetForm("SKAH") as Keyword
EndFunction
EndProperty

Keyword Property ClothingHands
Keyword Function Get()
	return (GetForm(0x31) as DefaultObjectManager).GetForm("SKCH") as Keyword
EndFunction
EndProperty

Keyword Property ArmorFeet
Keyword Function Get()
	return (GetForm(0x31) as DefaultObjectManager).GetForm("SKAF") as Keyword
EndFunction
EndProperty

Keyword Property ClothingFeet
Keyword Function Get()
	return (GetForm(0x31) as DefaultObjectManager).GetForm("SKCF") as Keyword
EndFunction
EndProperty

Keyword Property ArmorBody
Keyword Function Get()
	return (GetForm(0x31) as DefaultObjectManager).GetForm("SKAB") as Keyword
EndFunction
EndProperty

Keyword Property ClothingBody
Keyword Function Get()
	return (GetForm(0x31) as DefaultObjectManager).GetForm("SKCB") as Keyword
EndFunction
EndProperty

Keyword Property ArmorHead
Keyword Function Get()
	return (GetForm(0x31) as DefaultObjectManager).GetForm("SKAO") as Keyword
EndFunction
EndProperty

Keyword Property ClothingHead
Keyword Function Get()
	return (GetForm(0x31) as DefaultObjectManager).GetForm("SKCO") as Keyword
EndFunction
EndProperty
