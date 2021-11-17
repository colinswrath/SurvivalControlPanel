Scriptname Survival_Json Hidden

; List all profiles
string[] Function ListFiles() global native

; Test whether profile exists
bool Function Exists(string asFilePath) global native

; Save current profile
bool Function Save(string asFilePath) global native

; Load saved profile
bool Function Load(string asFilePath) global native

; Delete saved profile
bool Function Delete(string asFilePath) global native
