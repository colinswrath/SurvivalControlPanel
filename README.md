This is a basic plugin template for use with CommonLibSSE

## Requirements
* [CMake](https://cmake.org/)
	* Add this to your `PATH`
* [The Elder Scrolls V: Skyrim Special Edition](https://store.steampowered.com/app/489830)
* [Vcpkg](https://github.com/microsoft/vcpkg)
	* Add the environment variable `VCPKG_ROOT` with the value as the path to the folder containing vcpkg
* [Visual Studio Community 2022](https://visualstudio.microsoft.com/)
	* Desktop development with C++

## Building
```
git clone https://github.com/Exit-9B/ExamplePlugin-CommonLibSSE
cd ExamplePlugin-CommonLibSSE
git submodule init
git submodule update
cmake --preset vs2022-windows
cmake --build build --config Release
```

## Tips
* Run `cmake-gui build` to configure project variables
* Configure `CMAKE_INSTALL_PREFIX` to install files to a virtual directory in Mod Organizer 2
* Set `CMAKE_VS_INCLUDE_INSTALL_TO_DEFAULT_BUILD` to `ON` to automatically install the mod when building in Visual Studio
* Build the `package` target to automatically build and zip up your dll in a ready-to-distribute format
