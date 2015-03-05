# Civilization 4 XML importer

This tool will convert XML files from the XML Android format (per-languages files) to the non-standard format of Civilization IV. It is only compatible with UTF8.

## Details

The importer takes some parameters:
- Directories: From (input files dir), To (output dir), Trans (where the Android XML translations are located).
- Importtoall: 0/1: To
It need to have files in the folder Text/import/<lang>/. In fact, it just read android XML files (UTF8). It will detect the import/ subfolder and know the import languages accordingly. If the folder is "fr", then it's french; if it's pl, then it's polish; etc. If there are multiple language, it will import them one after another.

- First, the parser read the files in Text/, copy them to Text/imported/ and convert them to UTF8.
- Then, it read the Android XML files, read 'name' attributes (tag) and look for this tag in each file in Text/imported/.
- When the tag is found in a file, it goes down to the tag corresponding to the language it is currently importing.
For the example above, it will read <French>Mod montagnes</French> and compare it to the text in the Text/import/<lang>/ file tag it is computing.
If the import file tag is <string name="TXT_KEY_CONCEPT_MOUNTAINS_TEXT">Mod montagnes</string>, then it will find that strings are identical then go to the next tag.
The parser is also capable to read subtags, like

        <French>
                <Text>Mod montagnes</Text>
                <Gender>Female</Gender>
                <Plural>0</Plural>
        </French>

It will compare the subtag text with the android xml files.
If the values are different, it simply put the value from the android xml tag to the "Text/imported/" file. If the "Text/import/<lang>/" is empty, it will import an empty (this is preferable if someone removed a bad translation).

If the tag for the language doesn't exist, just create it and put the value.

 - Then it goes to the next tag in the same "Text/import/<lang>/" file it was reading. At the end of the file, it goes to the next file.

 - When it has read all the tags in "Text/import/<lang>/" files, it generate a report that contains lines like that:

        FILE: imported/A_New_Dawn_Concept_Civ4GameText.xml | TAG: TXT_KEY_CONCEPT_MOUNTAINS_TEXT | OLD: Berge-Mod -> NEW: qsqgdsqgdsg

 You can read the file concerned, the tag, the old value and the new value so you can quickly have an overview of what have changed !

 - The "Text/imported/" files are converted back to ISO-8859-1 to make sure that EVERY special character is encoded ("&#235;", "&amp;", etc.).

 - At the end of the process, it will generate a md5 checksum of all files in "Text/" and compare it with the one in "Text/imported/". If its identical, it remove the file in "Text/imported/". That way, it only keep the modified files in "Text/imported/". That's cleaner !

 #### Some notes:
 - Even if no values are modified, if the "Text/" contains a non-encoded special character, when the file is converted to UTF8 then back to ISO-8859-1, the special character will be encoded, and thus, the checksum of the files will be different but no report will be generated ! You won't see this behavior on the current mod files as i have converted every special character but it may appear with the new files you want to import. Just check with a merge editor like TortoiseMerge (installed with TortoiseSVN) or WinMerge and you will quickly see what have changed. To see that behavior, you can use the import function with no files to import, it will convert files back-and-forth so only the files with a non-encoded special character will last.

### Import to same file

This function is nearly identical as the classic import except it only check the files that have the same name.
If the file is "import/fr/A_New_Dawn_Concept_Civ4GameText.xml», then it will only check «A_New_Dawn_Concept_Civ4GameText.xml» in root folder instead of checking all files in root folder.

If you have duplicated tags and don't want to use the sort function, it is recommended to use this function.
