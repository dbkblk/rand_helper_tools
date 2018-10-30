# How to generate a full installer ?

## Requirements:
- NSIS Unicode (latest known is 2.46.5) must be installed (reboot after install). The setup is available in the sub-folder "NSIS_unicode_setup".

## Usage (Without updating translations):
Right-click on "full_installer_script.nsi" and select "Compile Unicode NSIS Script". It will set the revision number to the current revision of the repository and start the generation of the installer named "and2_full_installer_${DATE}_rev${REVISION}.exe".
Once done, you can either "Test installer" to launch it or close the window. The installer executable should appear next the to script file.

## Usage (With a translation update):
- First, it is needed to initialize transifex on your PC before to be able to download latest translations. Please read the read-me in the folder "Translations".
- Once done, double-click on "Update installer translations.bat". It will download the latest installer translation and inject those into the installer script (full_installer_script.nsi).
- Then, just Right-click on "full_installer_script.nsi", select "Compile Unicode NSIS Script" and wait for the installer to be generated.

## Some notes:
- The script strongly relies on the folder structure of the repository as it will pick icons and build number directly into the mod folder.
- The most important part to generate a build is the line 60 “; Language” which lists the enabled languages. All the translations are directly included in the files starting at line 99 “; Translations”.
Some of these lines are commented because the languages are not enabled yet (only partial or no translation yet).


dbkblk, 2015-09
