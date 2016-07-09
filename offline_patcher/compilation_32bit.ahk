compiler = C:\Program Files\AutoHotkey\Compiler\Ahk2Exe.exe

; Get version number
FileRead, ahk, rand_offline_patcher.ahk
startTxt = version = 
startLen := StrLen(startTxt)
stopTxt = ;_version
StringGetPos, startPos, ahk, %startTxt%
StringGetPos, stopPos, ahk, %stopTxt%
version := SubStr(ahk, startPos + startLen + 2, 3)

; Compilation
RunWait, "%compiler%" /in rand_offline_patcher.ahk /out "files\rand_offline_patcher.exe" /icon and_icon.ico /bin "C:\Program Files\AutoHotkey\Compiler\Unicode 32-bit.bin" /mpress 1

; Compression
SetWorkingDir, %A_ScriptDir%\files
RunWait, 7za a -tzip -r rand_offline_patcher_v%version%.zip *.*,,Hide
ExitApp