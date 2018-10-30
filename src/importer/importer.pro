#-------------------------------------------------
#
# Project created by QtCreator 2015-02-18T21:51:26
#
#-------------------------------------------------

QT       += core xml

QT       -= gui

TARGET = importer
CONFIG   += console qt

win32 {
    CONFIG  +=  static
    QMAKE_LFLAGS += -static-libgcc -static-libstdc++ -static -lstdc++ -lpthread
}

CONFIG   -= app_bundle

RC_FILE = icon.rc

SOURCES += main.cpp

DISTFILES += \
    README.md

DISTFILES +=

Release:DESTDIR = ../../build/release/importer
Release:OBJECTS_DIR = ../../build/release/importer/.obj
Release:MOC_DIR = ../../build/release/importer/.moc
Release:RCC_DIR = ../../build/release/importer/.rcc
Release:UI_DIR = ../../build/release/importer/.ui

Debug:DESTDIR = ../../build/debug/importer
Debug:OBJECTS_DIR = ../../build/debug/importer/.obj
Debug:MOC_DIR = ../../build/debug/importer/.moc
Debug:RCC_DIR = ../../build/debug/importer/.rcc
Debug:UI_DIR = ../../build/debug/importer/.ui