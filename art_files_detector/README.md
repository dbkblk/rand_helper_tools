# Civ. IV - XML Cleaner

## Usage
This function try to find tags in the mod text files which aren't used in the base game nor in the modded dll.

This tool will iterate trough the DLL source code files, then in every xml, python and WorldBuilderSave, to look for art files reference.
This is a little hacky because it may not find references for files that are only referenced by the game engine.

A report will be generated at the end.
