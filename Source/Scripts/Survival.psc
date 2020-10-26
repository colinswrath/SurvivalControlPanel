Scriptname Survival Hidden

; NOTE: You cannot actually access these variables from your scripts, so you
; will have to copy the definitions if you want to use them.
int Property HUD_INDICATORS    = 0  AutoReadOnly
int Property INVENTORY_UI      = 1  AutoReadOnly
int Property SLEEP_TO_LEVEL_UP = 2  AutoReadOnly
int Property ARROW_WEIGHT      = 3  AutoReadOnly
int Property LOCKPICK_WEIGHT   = 4  AutoReadOnly

; Enable the requested Survival feature unless overridden by the user.
Function ForceEnable(int aiFeature) global native

; Disable the requested Survival feature unless enabled by another mod or
; overridden by the user.
Function RequestDisable(int aiFeature) global native

; Undo mod configurations to the requested Survival feature, falling back to the
; default game behavior unless overridden by the user. This should only be
; called from the MCM.
Function ModReset(int aiFeature) global native

; Enable the requested Survival feature, overriding all mods. This should only
; be called from the MCM.
Function UserEnable(int aiFeature) global native

; Disable the requested Survival feature, overriding all mods. This should only
; be called from the MCM.
Function UserDisable(int aiFeature) global native

; Undo user overrides, falling back to either the default game behavior or
; behavior configured by mod scripts. This should only be called from the MCM.
Function UserReset(int aiFeature) global native

; Indicates whether the requested Survival feature was enabled by any mods.
bool Function IsEnabledByMods(int aiFeature) global native

; Indicates whether the requested Survival feature was disabled by any mods.
bool Function IsDisabledByMods(int aiFeature) global native

; Indicates whether the requested Survival feature was enabled by the user.
bool Function IsEnabledByUser(int aiFeature) global native

; Indicates whether the requested Survival feature was disabled by the user.
bool Function IsDisabledByUser(int aiFeature) global native

; Enables support for Frostfall keywords in Survival Mode.
Function EnableFrostfallKeywords() global native

; Disables support for Frostfall keywords in Survival Mode.
Function DisableFrostfallKeywords() global native

; Get whether Frostfall keywords for Survival Mode are enabled.
bool Function AreFrostfallKeywordsEnabled() global native

; Enables warmth ratings for cloaks (slot 46).
Function EnableCloakWarmth() global native

; Disables warmth ratings for cloaks (slot 46).
Function DisableCloakWarmth() global native

; Get whether warmth ratings are enabled for cloaks.
bool Function IsCloakWarmthEnabled() global native

; Sets the warmth rating cloaks should have by default (not classified as warm
; or cold).
Function SetCloakNormalBonus(float afWarmth) global native

; Sets the warmth rating cloaks should have if they are classified as warm.
Function SetCloakWarmBonus(float afWarmth) global native

; Sets the warmth rating cloaks should have if they are classified as cold.
Function SetCloakColdBonus(float afWarmth) global native

; Gets the warmth rating cloaks should have by default (not classified as warm
; or cold).
float Function GetCloakNormalBonus() global native

; Gets the warmth rating cloaks should have if they are classified as warm.
float Function GetCloakWarmBonus() global native

; Gets the warmth rating cloaks should have if they are classified as cold.
float Function GetCloakColdBonus() global native
