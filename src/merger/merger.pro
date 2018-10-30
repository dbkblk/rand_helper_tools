#-------------------------------------------------
#
# Project created by QtCreator 2015-02-18T21:51:39
#
#-------------------------------------------------

QT       += core xml

QT       -= gui

TARGET = merger
CONFIG   += console qt static
CONFIG   -= app_bundle

TEMPLATE = app

QMAKE_LFLAGS += -static-libgcc -static-libstdc++ -static -lstdc++ -lpthread

SOURCES += main.cpp

RC_FILE = icon.rc

DISTFILES += \
    README.md

DISTFILES +=

Release:DESTDIR = ../../build/release/merger
Release:OBJECTS_DIR = ../../build/release/merger/.obj
Release:MOC_DIR = ../../build/release/merger/.moc
Release:RCC_DIR = ../../build/release/merger/.rcc
Release:UI_DIR = ../../build/release/merger/.ui

Debug:DESTDIR = ../../build/debug/merger
Debug:OBJECTS_DIR = ../../build/debug/merger/.obj
Debug:MOC_DIR = ../../build/debug/merger/.moc
Debug:RCC_DIR = ../../build/debug/merger/.rcc
Debug:UI_DIR = ../../build/debug/merger/.ui
