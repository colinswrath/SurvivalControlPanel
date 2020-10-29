Scriptname Survival_JContainers Hidden

; Indicates whether JContainers is available and successfully initialized
bool Function IsAvailable() global native

; Save current settings to file
Function Save(String asFilePath) global native

; Load settings from saved file
Function Load(String asFilePath) global native
