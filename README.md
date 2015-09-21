# Civilization 4 Helper tools

This repository contains all the tools that have been developped to support the developement of the mod Civ. IV Rise of Mankind - A New Dawn http://anewdawn.sf.net

Please refer to the readme into each tool folder.

## Some tricks using some of the tools:
### Merge all base game strings to the platform:

If you remember how the import function work, you'll know that the last imported value will takeover the original value. So, you can merge the base game strings this way:
- Create an empty folder anywhere
- Copy the parser in it (with the categories file)
- Put base civ 4 xml text files
- Start the parser, export all languages
- Remove base civ 4 xml text files
- Save the exported folder
- Do this for warlords and bts separately and backup export each time
- Also do this for the mod files
- Now put all the files in the same folder (base, warlords, bts and mod), and use the sorting function (all tags will be merged and many duplicates will be created but that's not important at this step). This step last 7 hours on my pc but it will be improved.
- Removed the original files
- Now put the sorted files in the root folder again
- Take the warlords file (because this is first expansion) and put the languages files (android xml) to "import/" subfolder
- Import file with the parser (the warlords strings will takeover all the known tags in the sorted files)
- Replace originals by imported
- Do this in this order for all mods (Warlords > BTS > Mod), so only the last strings are kept

At the end, you will have sorted files with the last strings values known. That's why removed files are not important to keep for this merge process. CAUTION: DO NOT DO THIS ON FILE MOD NOW... because i've already done this, so even the base game text can be edited on the platform. As the clean operation, it only need to be done one time.

### Merge new text files to the category files:

- Create an empty folder anywhere
- Put the mod files and the new text files you want to merge
- Copy the parser in it (with the categories file)
- Start the parser and sort files. Go read a book while processing :D
- At the end, check the "_REMOVED_TAGS_CATEGORY.xml" files to see if duplicates have been found and manually choose what tag values to keep.
- The sorted files are in "sorted/". Just replace original.
