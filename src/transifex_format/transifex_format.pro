#-------------------------------------------------
#
# Project created by QtCreator 2015-02-07T11:44:53
#
#-------------------------------------------------

QT       += core xml

QT       -= gui

TARGET = transifex_format
CONFIG   += console static
CONFIG   -= app_bundle

TEMPLATE = app

RC_FILE = icon.rc

QMAKE_LFLAGS += -static-libgcc -static-libstdc++ -static -lstdc++ -lpthread

SOURCES += main.cpp

DISTFILES += \
    README.md

DISTFILES +=

Release:DESTDIR = ../../build/release/transifex_format
Release:OBJECTS_DIR = ../../build/release/transifex_format/.obj
Release:MOC_DIR = ../../build/release/transifex_format/.moc
Release:RCC_DIR = ../../build/release/transifex_format/.rcc
Release:UI_DIR = ../../build/release/transifex_format/.ui

Debug:DESTDIR = ../../build/debug/transifex_format
Debug:OBJECTS_DIR = ../../build/debug/transifex_format/.obj
Debug:MOC_DIR = ../../build/debug/transifex_format/.moc
Debug:RCC_DIR = ../../build/debug/transifex_format/.rcc
Debug:UI_DIR = ../../build/debug/transifex_format/.ui