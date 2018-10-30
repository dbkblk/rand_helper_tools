#-------------------------------------------------
#
# Project created by QtCreator 2015-02-23T08:47:52
#
#-------------------------------------------------

QT       += core xml

QT       -= gui

TARGET = script_translations
CONFIG   += console qt static
CONFIG   -= app_bundle

TEMPLATE = app

RC_FILE = manifest.rc

QMAKE_LFLAGS += -static-libgcc -static-libstdc++ -static -lstdc++ -lpthread

SOURCES += main.cpp

DISTFILES += \
    README.md

DISTFILES +=

Release:DESTDIR = ../../build/release/script_translations
Release:OBJECTS_DIR = ../../build/release/script_translations/.obj
Release:MOC_DIR = ../../build/release/script_translations/.moc
Release:RCC_DIR = ../../build/release/script_translations/.rcc
Release:UI_DIR = ../../build/release/script_translations/.ui

Debug:DESTDIR = ../../build/debug/script_translations
Debug:OBJECTS_DIR = ../../build/debug/script_translations/.obj
Debug:MOC_DIR = ../../build/debug/script_translations/.moc
Debug:RCC_DIR = ../../build/debug/script_translations/.rcc
Debug:UI_DIR = ../../build/debug/script_translations/.ui