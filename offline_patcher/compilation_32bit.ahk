compiler = C:\Program Files\AutoHotkey\Compiler\Ahk2Exe.exe

; Get version number
fahk = rand_offline_patcher.ahk
FileRead, ahk, %fahk%
RegExMatch(ahk, "version = (.*)", v)

; Compilation
RunWait, "%compiler%" /in %fahk% /out "files\rand_offline_patcher.exe" /icon and_icon.ico /bin "C:\Program Files\AutoHotkey\Compiler\Unicode 32-bit.bin" /mpress 1

; Compression
SetWorkingDir, %A_ScriptDir%\files
RunWait, 7za a -tzip -r rand_offline_patcher_v%v1%.zip *.*,,Hide
SetWorkingDir, %A_ScriptDir%
FileMove, %A_ScriptDir%\files\rand_offline_patcher_v%v1%.zip, %A_ScriptDir%\archives\rand_offline_patcher_v%v1%.zip, 1
ExitApp