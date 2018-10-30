#!/bin/bash
# Author: Hadrien Dussuel
# Date: 20170428
# Description: Quickly rebuild all the linux version of the tools and put the executables in build_linux64

EXPORT_DIR="/build/"

function rebuild {
  echo "
  ** Building $1 for linux64 **
  "
  cd $1
  qmake *.pro
  make -j3 -s
  make clean -s
  rm -f Makefile*
  mv $1 $EXPORT_DIR/
  cd ..
}
rebuild exporter
rebuild importer
rebuild merger
rm -f Makefile*

