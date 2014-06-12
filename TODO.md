TODO:
DONE - Activate the importation of empty tags
DONE - Recheck the output if subtag (output but don't do anything)
DONE - Fix clean up
- Function: Find unused tags
- Misc report after sorting
- Find tag in all files

## Process to merge the game files
- Merge with the game files
        -> Sort all files then merge back
        -> Export all Civ 4 Warlords files
        -> Import all Civ 4 Warlords to plain game
        -> Export all Civ 4 BTS files
        -> Import all Civ 4 BTS to newly warlords
        -> Export all "french" and "polish" updates
        -> Import the translated strings to new files

Known problem: For some reason, the character "&" (ampersand) (ex: Rock & Roll) is breaking the clean up function. The file is cut. Be careful when using this function. You can workaround this by replacing & with "&amp;" (ex: Rock &amp; Roll). Tip: To easily find an ampersand, search for "& " (ampersand + a space) in the document in the original files listed in the report. You can also use a program like TextCrawler. Once you've removed these in the original files, you can safely launch the cleaning again.

