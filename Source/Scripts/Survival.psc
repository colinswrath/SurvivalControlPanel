Scriptname Survival Hidden

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
