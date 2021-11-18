@echo off

cmake -B build -S . --check-stamp-file "build/CMakeFiles/generate.stamp"
if %ERRORLEVEL% NEQ 0 exit 1
cmake --build build --config Release
if %ERRORLEVEL% NEQ 0 exit 1

xcopy "dist\Data" "%2\SkyrimSE" /i /s /y

xcopy "build\Release\%1.dll" "%2\SkyrimSE\SKSE\Plugins\%1.dll*" /y
