#NoTrayIcon

version = 2.1
tool_svn = %A_ScriptDir%\tools\svn.exe
tool_diff = %A_ScriptDir%\tools\diff.exe

; Checking ini file
settings = %A_ScriptDir%\settings.ini
IfExist %settings%
{
    IniRead, sGameDir, %settings%, game, dir
}

; Check game dir
IfNotExist, %sGameDir%\Assets\Python\Contrib\CvModName.py
{
    sGameDir = 
}


; Titles
Gui, font,bold
Gui, Add, Text, x10 y10 w280 h25, 1 - Patch downloader
Gui, Add, Text, x10 y200 w280 h25, 2 - Apply patch
Gui, font

; Text details
Gui, Add, Text, x10 y35 w280, Use this on a PC with an internet connexion. Enter the revision number of your offline installation (upper right in the launcher screen), then press Download. The program will grab all the updates in a patch file.
Gui, Add, Text, x10 y225 w280, Use this on the PC without internet. Press Apply, select the patch file, then select the mod folder installation, usually My Documents/My Games/Mods/Rise of Mankind - A New Dawn. Wait a moment for the process to execute.

; Add elements
Gui, Add, Edit, x100 y100 w100 h25 vrev, Enter rev. number
Gui, Add, Button, x100 y130 w100 h25 gdownload vbtnDown, Download
Gui, Add, Text, x10 y160 w150 h25 vTxtProgress, Download in progress...
GuiControl, Hide, TxtProgress
Gui, Add, Button, x100 y290 w100 h25 gapply, Apply
Gui, Add, Text, x260 y365 w50 h25, v%version%
Gui, Show, w300 h400, RAND offline patcher
return

download:
{
    ; Get edit box value and check if it's a number
    GuiControlGet, rev
    if rev is not number
    {
        MsgBox, "%rev%" is not a revision number
    } else
    {
        ; Disable the download button
        GuiControl, Disable, btnDown
        GuiControl, Show, TxtProgress

        ; Start the svn diff command
        command = "%tool_svn%" diff -r%rev% --force --diff-cmd "%tool_diff%" -x -au "http://svn.code.sf.net/p/anewdawn/code/Trunk/Rise of Mankind - A New Dawn/" > and_%rev%_update.patch
        RunWait, %comspec% /c "%command%",,Hide

        ; Check if file exists
        IfExist and_%rev%_update.patch
        {
            MsgBox, , Download finished, Patch (rev%rev% -> latest) has been downloaded.
        } else 
        {
            MsgBox, , Something has gone wrong, There was an error while downloading the patch.
        }

        ; Enable the button again
        GuiControl, Enable, btnDown
        GuiControl, Hide, TxtProgress
    }
        
}
return

apply:
{
    ; Ask for patch file
    FileSelectFile, fPatch, , %A_ScriptDir%, Select the patch file to apply, *.patch
    if fPatch = 
    {
        return
    } else
    {
        ; Select game dir if not already registered
        if sGameDir =
        {
            FileSelectFolder, sGameDir, , %A_ScriptDir%, Select the folder where the mod is installed
            IfNotExist, %sGameDir%\Assets\Python\Contrib\CvModName.py
            {
                MsgBox, Wrong folder selected or canceled.
                sGameDir = 
                return
            }
            IniWrite, %sGameDir%, %settings%, game, dir
        }

        ; Get patch revision
        SplitPath, fPatch, fPatchName
        StringSplit, fPatchArray, fPatchName, "_"
        sPatchRev = %fPatchArray2%

        ; Get local revision
        FileRead, fMod, %sGameDir%\Assets\Python\Contrib\CvModName.py
        startTxt = modVersion = "2.2 (rev
        startLen := StrLen(startTxt)
        stopTxt = )"
        StringGetPos, startPos, fMod, %startTxt%
        StringGetPos, stopPos, fMod, %stopTxt%
        sLocalRev := SubStr(fMod, startPos + startLen + 1, stopPos - (startPos + startLen))

        ; Get latest revision
        FileRead, fPatchFile, %fPatch%
        startTxt = +modVersion = "2.2 (rev
        startLen := StrLen(startTxt)
        stopTxt = )"
        StringGetPos, startPos, fPatchFile, %startTxt%
        sUpdateRev := SubStr(fPatchFile, startPos + startLen + 1, startPos + startLen + 4 - (startPos + startLen))

        ; Check if the patch rev is correct
        If (sLocalRev <> sPatchRev)
        {
            MsgBox, , Cannot patch the mod, The patch downloaded (rev%sPatchRev%) cannot be applied on the mod directory (rev%sLocalRev%)
            return
        }

        ; Prepare to patch
        SetWorkingDir, %sGameDir%
        cmdPatch = %A_ScriptDir%\tools\apply.exe -p0 -E -N --binary --input=%fPatch%
        RunWait, %comspec% /c "%cmdPatch%" && timeout 5,,
        
        ; Clear the cache
        EnvGet, appdata, LOCALAPPDATA
        cacheDir = %appdata%\My Games\beyond the sword\cache
        FileRemoveDir, %cacheDir%, 1

        ; Warn the user
        MsgBox, , Done, Patch applied.
    }
}
return

GuiClose:
ExitApp 