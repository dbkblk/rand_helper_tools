# Civilization 4 XML translation tool

*This tool is intended to be used by developpers*

## DESCRIPTION
The goal of this tool is to convert civ 4 xml text files to individual language xml file. These individual language files can be imported to translation platform. 
It can import back the modified strings and generate a report.

## HOW IT WORKS
As of v0.5, it has 3 functions enabled. To work, the executable should be copied in the same folder as xml files to play with. Start the executable and follow the menu description.

## Functions
### Export all
The parser read every xml text file in the folder and extract every string for every language known. It create a subfolder «lang» which contains a folder for the 9 main languages actually supported: English, Finnish, French, German, Italian, Hungarian, Polish, Russian* and Spanish. 

* The russian language is specific as it does not use ISO-8859-1 but ISO-8859-5 characters. The charset used in the xml files is latin1 with modded GameFonts.tga and thus, the files are converted to UTF8, so the output files are perfectly readable by russians (but not by me, unfortunately :D).

For example, the file «A_New_Dawn_Concept_Civ4GameText.xml» will be spread in 6 files for each language which will contains only one language.

The input civ 4 format look like:

	<TEXT>
		<Tag>TXT_KEY_CONCEPT_MOUNTAINS_TEXT</Tag>
		<English>Mountains Mod</English>
		<French>Mod montagnes</French>
		<German>Berge-Mod</German>
		<Italian>Mountains Mod</Italian>
		<Spanish>Mountains Mod</Spanish>
		<Polish>Eksploatacja Gór</Polish>
	</TEXT>

The exported format is, for example, in polish langage:

	<string name="TXT_KEY_CONCEPT_MOUNTAINS_TEXT">Eksploatacja Gór</string>
	
Same example in french:

	<string name="TXT_KEY_CONCEPT_MOUNTAINS_TEXT">Mod montagnes</string>

To resume, the function do:
- Read all the xml files in the folder where the parser is present. The input files are ISO-8859-1 encoded (Civilization IV format).
- Extract each string for each language and generate a subfolder "lang/" with a subfolder for each language (en, fr, de, etc.).
- Russian is converted from encoded based on the modded GameFonts.tga and converted to readable UTF8.
- Each language subfolder will contain a file formatted as the last example with the same name as the input file. The output files are UTF8.

### Export a language

Same function as behind but it only extract one language.

### Import a language

The import function is the reverse of export function with enhancements !
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

### Tag sorting

The goal of this one is extremely important as it keep the "Text/" folder clean. The parser need to have a "_categories.parse" is the same folder as the executable. Basically, it's just a XML file with another extension.
Inside this file, you can read:

	<?xml version="1.0" encoding="utf-8"?>
	<main>
	  <category file="DIPLOMACY">
	    <tag>AI_</tag>
	    <tag>USER_</tag>
	    <tag>TXT_KEY_MISC_ATTITUDE</tag>
	    <tag>TXT_KEY_TRADE</tag>
	  </category>
	  <category file="BUILDINGS">
	    <tag>MOD_TXT_KEY_BUILDING</tag>
	    <tag>TXT_KEY_BUILDING</tag>
	  </category>
	  ...
	</main>

It is composed of a tag "category" with an attribute "file", which is the name of the category. Below, there are tag with prefix value "AI_", "USER_". That mean, when a parser will read a tag in "Text/" files, if the tag begin with that prefix, it will consider that the tag is in the category "DIPLOMACY". I've tried to make this easily editable for modders.

Now, let's see how it works:
- The parser read all files in "Text/" and list all tags.
- It start with the first tag in the list, look for it.
- When it's found, it add all the tags and subtags in the corresponding category we've seen just before. For example,

	<TEXT>
	  <Tag>AI_DIPLO_ACCEPT_1</Tag>
	  <English>Agreed.</English>
	  <French>C'est d'accord.</French>
	  <German>Einverstanden!</German>
	  <Italian>D'accordo.</Italian>
	  <Spanish>Hecho.</Spanish>
	  <Finnish>Hyv&#228;ksytty.</Finnish>
	  <Hungarian>Megegyezt&#252;nk.</Hungarian>
	  <Polish>Zgoda.</Polish>
	  <Russian>&#209;&#238;&#227;&#235;&#224;&#241;&#229;&#237;.</Russian>
	 </TEXT>

... will be added in the file "DIPLOMACY.xml" because the tag begin with "AI_". All the language tags and subtags will be added as well.

If the tag contains "_PEDIA" in its name, it is moved in "CATEGORY_PEDIA.xml" so we can split the essential translations from civilopedia translations. For example,

	 <TEXT>
	  <Tag>TXT_KEY_BUILDING_ABU_SIMBEL_PEDIA</Tag>
	  <English>The Abu Simbel temples are two huge rock temples in southern Egypt. The temples were carved from mountainsides between 1264 and 1244 BCE, built by Pharoah Ramesses II to commemorate the Battle of Kadesh. In the 1960's, the entire temples were dug up and relocated to avoid their being submerged in the lake created by the Aswan High Dam.</English>
	  <French>Les temples d'Abou Simbel sont deux temples de roche &#233;normes dans le sud de l'Egypte. Les temples ont &#233;t&#233; sculpt&#233;s aux flancs entre 1264 et 1244 avant notre &#232;re, construit par le pharaon Rams&#232;s II pour comm&#233;morer la bataille de Qadesh. Dans les ann&#233;es 1960, des temples entiers ont &#233;t&#233; d&#233;terr&#233;s et d&#233;plac&#233;s pour &#233;viter d'&#234;tre submerg&#233;s dans le lac cr&#233;&#233; par le barrage d'Assouan.</French>
	 </TEXT>

.. is in "BUILDINGS" category...

	<category file="BUILDINGS">
		<tag>MOD_TXT_KEY_BUILDING</tag>
		<tag>TXT_KEY_BUILDING</tag>
	</category>
  
  ... but have been moved to "BUILDINGS_PEDIA.xml" file.
  
  - Then it continue to scan files to check if the tags is present multiple times. Now, what happens if the tag is found two times ?
  The second time, instead of copying the tag to the "CATEGORY.xml", it is copied to "_REMOVED_TAGS_CATEGORY.xml", so only ONE version of the tag is kept in "CATEGORY.xml". The good news is that you'll never get any duplicated tag with that method ! The bad news is that you will need to open each "_REMOVED_TAGS_CATEGORY.xml" and compare the values for the same tag in "CATEGORY.xml" because the parser can't know which language value you have to keep.
  
  - At the end of the process, a report is generated. For example: "TAG:AI_DIPLO_ACCEPT_1 | FILE:DIPLOMACY.xml" will mean that you will find "AI_DIPLO_ACCEPT_1" in "DIPLOMACY.xml" and in "_REMOVED_TAGS_DIPLOMACY.xml" so you'll have to check manually which value to keep (they are often identical so this is fast to check it out).

### Tag cleaning

The goal is to removed all languages tags that are empty or identical to english tags. DO NOT USE this one. I add to build this to remove some values.
 In the past, some english tags have been copied in each language but this is a bad behavior. By default, the game use the english tag if no value is specified for the language. There is no need to have the english copied in each language tag, because it makes the xml files heavier (game is starting slower), it makes the translation platform to show incorrect percentage of translation because it thinks that the string is translated whereas it's english. 
 So, the function do:
 - Copy "Text/" to "Text/cleaned/"

 - Convert to UTF8

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

- Convert back to ISO-8859-1

- Check checksums and keep only modified files.

- Generate report which contains lines like "FILE: BUILDINGS.xml - TAG: TXT_KEY_BUILDING_AIRPORT_STRATEGY - Removing Russian (empty)" or "FILE: BUILDINGS.xml - TAG: TXT_KEY_BUILDING_ROMAN_FORUM - Removing French (identical)". I think this is pretty straightforward.

# Some use cases

## Merge all base game strings to the platform:

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

## Merge new text files to the category files:

- Create an empty folder anywhere
- Put the mod files and the new text files you want to merge
- Copy the parser in it (with the categories file)
- Start the parser and sort files. Go read a book while processing :D
- At the end, check the "_REMOVED_TAGS_CATEGORY.xml" files to see if duplicates have been found and manually choose what tag values to keep.
- The sorted files are in "sorted/". Just replace original.

## KNOWN PROBLEMS

If a tag to import is used multiple times in original files (really, that shouldn't be that way), the import function will inject the value from the import file to every original file.

Example:
The TAG: TXT_BLABLA is in original file A and original file B
The values are
Original file A: BABA
Original file B: BIBI

If a file to import contains the tag TXT_BLABLA with the value «BABA» (even if this tag haven't been modified during the export and the import) the value «BABA» will be injected to file A and to file B.

I plan to add functions to detect and remove duplicates tag.

## FURTHER DEVELOPMENT

-> Backup and remove a specific language
-> Detect unused tag based on python and xml files in «Assets»

[URL="https://dl.dropboxusercontent.com/u/369241/Civ4-XMLParser_v0.5.7z"]DOWNLOAD LINK (v0.5)[/URL]

[URL="https://github.com/dbkblk/civ4_xml_parser/"]GITHUB LINK[/URL]
