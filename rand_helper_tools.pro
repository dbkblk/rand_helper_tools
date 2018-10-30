TEMPLATE = subdirs

SUBDIRS = src/art_files_detector src/exporter src/importer src/merger src/cleaner src/audio_injector src/transifex_format \
    src/script_translations src/mcp_installer_lang_update src/asian_patcher \
    src/scan_tool \
    src/checksum

art_files_detector = src/art_files_detector/art_files_detector.pro
asian_patcher = src/asian_patcher/asian_patcher.pro
audio_injector = src/audio_injector/audio_injector.pro
checksum = src/checksum/checksum.pro
cleaner = src/cleaner/cleaner.pro
exporter.file = src/exporter/exporter.pro
importer = src/importer/importer.pro
script_translations = src/script_translations/script_translations.pro
mcp_script_translations = src/mcp_script_translations/mcp_script_translations.pro
merger = src/merger/merger.pro
scan_tool = src/scan_tool/scan_tool.pro
transifex_format = src/transifex_format/transifex_format.pro

DISTFILES += README.md\
            TODO.md
