TEMPLATE = subdirs

SUBDIRS = exporter importer merger cleaner audio_injector offline_patcher transifex_stats_parser mcp_updater

exporter.file = exporter/exporter.pro
importer = importer/importer.pro
merger = merger/merger.pro
cleaner = cleaner/cleaner.pro
audio_injector = audio_injector/audio_injector.pro
offline_patcher = offline_patcher/offline_patcher.pro
transifex_stats_parser = transifex_stats_parser/transifex_stats_parser.pro
mcp_updater = mcp_updater/mcp_updater.pro

DISTFILES += README.md\
            TODO.md
