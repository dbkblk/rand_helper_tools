# Civ. IV - Audio injector

## Usage
As Civilization IV is modular, it is possible to use components as sub-mod into a mod (placed in Modules/). The game will load all the xml files and merge them dynamically at launch. However, audio xml files are not merged.
This executable will check mod's audio xml file and inject those of sub-mods. When a submod is removed, audio injector will remove xml references into the mod audio files, to keep them clean.

## Requirements
Nothing

## Execution
Just put it in the Modules/ folder and execute. With Civ. IV, A New Dawn, it is automatically launcher at each launcher start, thus avoiding the need for players to use it manually.
