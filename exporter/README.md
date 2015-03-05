# Civilization 4 XML exporter

This tool will convert XML files from the non-standard format of Civilization IV to the XML Android format (per-languages files). It is only compatible with UTF8.

## Details

The parser read every xml text file in the folder and extract every string for every language known. It create a subfolder «lang» which contains a folder for all the enables languages.

For example, the file «A_New_Dawn_Concept_Civ4GameText.xml» will be spread in x files for each language which will contains only one language.

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

All the configuration is available in "exporter.config".

To resume, the exporter does:
- Read all the xml files in the folder where the parser is present. The input files are UTF-8 encoded.
- Extract each string for each language and generate a subfolder with another subfolder for each language (en, fr, de, etc.) (configurable in exporter.config).
- Each language subfolder will contain a file formatted as the last example with the same name as the input file. The output files are UTF8.
