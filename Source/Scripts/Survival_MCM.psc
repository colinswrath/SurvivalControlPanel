Scriptname Survival_MCM extends SKI_ConfigBase

import Survival

int Property DEFAULT_INDEX  = 0  AutoReadOnly
int Property DISABLED_INDEX = 1  AutoReadOnly
int Property ENABLED_INDEX  = 2  AutoReadOnly

String[] MenuEntries
int[] MenuOptions

Event OnInit()
	parent.OnInit()

	MenuEntries = new String[3]
	MenuEntries[DEFAULT_INDEX] = "$Default"
	MenuEntries[DISABLED_INDEX] = "$Always Disabled"
	MenuEntries[ENABLED_INDEX] = "$Always Enabled"

	MenuOptions = new int[5]
EndEvent

Event OnPageReset(String a_page)
	SetCursorFillMode(TOP_TO_BOTTOM)

	AddHeaderOption("$Survival Mode Features")

	AddToggleOptions("$HUD indicators", 0)
	AddToggleOptions("$Inventory UI", 1)
	AddToggleOptions("$Sleep to level up", 2)
	AddToggleOptions("$Arrow weight", 3)
	AddToggleOptions("$Lockpick weight", 4)
EndEvent

Function AddToggleOptions(String asName, int aiFeature)

	bool bUserEnabled = IsEnabledByUser(aiFeature)
	bool bUserDisabled = IsDisabledByUser(aiFeature)
	bool bModEnabled = IsEnabledByMods(aiFeature)
	bool bModDisabled = IsDisabledByMods(aiFeature)

	if bUserEnabled
		MenuOptions[aiFeature] = AddMenuOption(asName, "$Always Enabled")
	elseif bUserDisabled
		MenuOptions[aiFeature] = AddMenuOption(asName, "$Always Disabled")
	else
		MenuOptions[aiFeature] = AddMenuOption(asName, "$Default")
	endif
EndFunction

Event OnOptionMenuOpen(int option)
	int iFeature = 0
	while option != MenuOptions[iFeature] && iFeature < 5
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

Event OnOptionMenuAccept(int option, int index)
	int iFeature = 0
	while option != MenuOptions[iFeature] && iFeature < 5
		iFeature += 1
	endwhile

	if index == DEFAULT_INDEX
		UserReset(iFeature)
		SetMenuOptionValue(option, MenuEntries[DEFAULT_INDEX])
	elseif index == DISABLED_INDEX
		UserDisable(iFeature)
		SetMenuOptionValue(option, MenuEntries[DISABLED_INDEX])
	elseif index == ENABLED_INDEX
		UserEnable(iFeature)
		SetMenuOptionValue(option, MenuEntries[ENABLED_INDEX])
	endif
EndEvent
