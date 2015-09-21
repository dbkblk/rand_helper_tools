# Civ. IV - XML Cleaner

## Usage
This tool will remove all languages tags that are empty or identical to english tags, to deeply clean mod text files or duplicated strings. DO NOT USE this one on an already cleaned game.
 In the past, some english tags have been copied in each language but this is a bad behavior. By default, the game uses the english tag if no value is specified for the language. There is no need to have the english copied in each language tag, because it makes the xml files heavier (game is starting slower), it also makes it difficult to overview the state of the translation in one language.
 Historically, the cleaner used to convert ISO-8859-1 files to UTF8 to do the conversion and to convert it back at the end. However, now our mod is using full UTF8, this isn't the case anymore.

### Tag cleaning function
 - Copy "Text/" to "Text/cleaned/"
 - Check every language tag value and compare it to English. If the test is positive, removed language tags.
For example, for:

        <TEXT>
                <Tag>TXT_KEY_CONCEPT_MOUNTAINS_TEXT</Tag>
                <English>Mountains Mod</English>
                <French>Mountains Mod</French>
                <German>Mountains Mod</German>
        </TEXT>

... the resulting files will have:

        <TEXT>
                <Tag>TXT_KEY_CONCEPT_MOUNTAINS_TEXT</Tag>
                <English>Mountains Mod</English>
        </TEXT>

- If the language contains empty tag, remove it:
For example, for:

        <TEXT>
                <Tag>TXT_KEY_CONCEPT_MOUNTAINS_TEXT</Tag>
                <English>Mountains Mod</English>
                <French>Mod montagnes</French>
                <German></German>
        </TEXT>

... the resulting files will have:

         <TEXT>
                <Tag>TXT_KEY_CONCEPT_MOUNTAINS_TEXT</Tag>
                <English>Mountains Mod</English>
                <French>Mod montagnes</French>
        </TEXT>

- Check checksums and keep only modified files.

- Generate report which contains lines like "FILE: BUILDINGS.xml - TAG: TXT_KEY_BUILDING_AIRPORT_STRATEGY - Removing Russian (empty)" or "FILE: BUILDINGS.xml - TAG: TXT_KEY_BUILDING_ROMAN_FORUM - Removing French (identical)". I think this is pretty straightforward.

### Find unused tags function

This function try to find tags in the mod text files which aren't used in the base game nor in the modded dll.

It will iterate trough the DLL source code files, then in every xml, python and WorldBuilderSave, to look for tags.
This is a little hacky because it cannot find tags which are generated on the fly by the game engine. You need to set exceptions for this kind of tag...
In the config file, set the prefix of theses tags. BUG and BUFFY are known to be generated on-the-fly, that's why there are already added to this list.

    <exceptions>
        <tag>TXT_KEY_BUG_OPT</tag>
        <tag>TXT_KEY_BUFFY</tag>
    </exceptions>

A report will be generated at the end.

### Remove a specific language function

The parser will check each game file and remove the specified language whenever it is encountered. A report will be generated.

## Requirements
Nothing

## Configuration
Tweak the parser.config file, which should be besides the executable.
The "directory" tag have some attributes, which tell the cleaner where to get and put files.
- "text", is the folder where to clean files
- "to, is the output directory
- "bts", is the base game directory (CIV IV base folder with a "Beyond the Sword" subfolder. WARNING: Do not point the BTS one directly)
- "sources", is your mod DLL source folder
- "mod", is the base mod folder

The "list" tag is the name of the report generated at the end of the process
The "exceptions" tag is a list of hardcoded tags that you should not be removed. They are basically hardcoded into the executable and cannot be detected by the cleaner, which would have wanted to remove them without this method.

Then just execute the cleaner and wait.

