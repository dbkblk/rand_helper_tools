#!/bin/bash
# Author: Hadrien Dussuel
# Date: 20170428
# Description: Quickly rebuild all the windows version of the tools and put the executables in build_win32

MXE_QMAKE="/home/hadrien/archives/src/mxe/usr/i686-w64-mingw32.static/qt5/bin/qmake"
EXPORT_DIR="/home/hadrien/archives/dev/civ4_svn_repository"

function rebuild {
  echo "
  ** Building $1 for win32 **
  "
  cd $1
  $MXE_QMAKE *.pro
  make -j3 -s
  make clean -s
  rm Makefile* -f
  upx -9 release/$1.exe
  mv release/$1.exe $EXPORT_DIR/Tools/
  rm -R -f release/ debug/
  cd ..
}

rebuild exporter
rebuild importer
rebuild merger
rm -f Makefile*
