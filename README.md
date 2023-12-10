# Backported Extended ESL Support

A Skyrim Special Edition Script Extender plugin that adds support for the extended plugin (ESL) range introduced in game version 1.6.1130 to versions 1.5.97, 1.6.659, and anything in between.

## Building

- CMake and vcpkg are expected to be set up beforehand. Visual Studio 2022 is recommended.
- \<preset\> is `skse`.
- \<build_preset\> is `skse-release` or `skse-debug`.

```
git clone --recurse-submodules https://github.com/Nukem9/skyrimse-backported-esl-support.git
cmake --preset <preset>
cmake --build --preset <build_preset>
```

## Installation

- For developers, edit `CMakeUserEnvVars.json` and set `GAME_ROOT_DIRECTORY` to Skyrim's root directory. The build script will automatically copy library files to the game folder.

- For manual SKSE installs, place `BackportedESLSupport.dll` in the corresponding `Skyrim Special Edition\Data\SKSE\Plugins` folder. An example path is: `C:\steamapps\common\Skyrim Special Edition\Data\SKSE\Plugins\BackportedESLSupport.dll`

## Purpose

- On December 5, 2023, Bethesda released the Creations update for Skyrim Special Edition. Not only did this break compatibility with SKSE plugins, it also broke compatibility with regular plugins (.ESP, .ESL, .ESM) by incrementing their header versions from 1.70 to 1.71. Attempting to use a version 1.71 plugin on an old game build will likely crash. So why did they do this? To expand the maximum number of new records in an ESL from 2048 to 4096.

- This aims to fix the crash mentioned above by backporting compatibility for 1.71 headers.

## Logs

- Diagnostic information is logged to `BackportedESLSupport.log` in `Documents\My Games\Skyrim Special Edition\SKSE` which lists any relevant plugins.

## License

- [GPLv3](COPYING) with [exceptions](EXCEPTIONS).
- Dependencies are under their respective licenses.