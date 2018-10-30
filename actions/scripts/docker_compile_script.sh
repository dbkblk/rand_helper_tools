#!/bin/bash
# Author: Hadrien Dussuel
# Date: 20181030
# Description: Quickly rebuild all the windows version of the tools and put the executables in build_win32

MXE_QMAKE="i686-w64-mingw32.static-qmake-qt5"

function rebuild {
  echo "
  ** Building $1 for win32 **
  "

  cd /app/src/$1
  $MXE_QMAKE *.pro
  make -j3 -s
  cd /app/

  upx -9 build/release/$1/$1.exe
  mv build/release/$1/$1.exe build/release/
}

rebuild art_files_detector
rebuild asian_patcher
rebuild audio_injector
rebuild checksum
rebuild cleaner
rebuild exporter
rebuild importer
rebuild script_translations
rebuild mcp_script_translations
rebuild merger
rebuild scan_tool
rebuild transifex_format

# echo "## PREPARING PROJECT ##"
# i686-w64-mingw32.static-qmake-qt5 rand_helper_tools.pro
# echo "## COMPILATION ##"
# make -j4