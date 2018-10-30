#-------------------------------------------------
#
# Project created by QtCreator 2015-02-18T21:51:56
#
#-------------------------------------------------

QT       += core xml

QT       -= gui

TARGET = cleaner
CONFIG   += console qt static
CONFIG   -= app_bundle

TEMPLATE = app

QMAKE_LFLAGS += -static-libgcc -static-libstdc++ -static -lstdc++ -lpthread

RC_FILE = icon.rc

SOURCES += main.cpp

DISTFILES += \
    README.md

DISTFILES +=

Release:DESTDIR = ../../build/release/cleaner
Release:OBJECTS_DIR = ../../build/release/cleaner/.obj
Release:MOC_DIR = ../../build/release/cleaner/.moc
Release:RCC_DIR = ../../build/release/cleaner/.rcc
Release:UI_DIR = ../../build/release/cleaner/.ui

Debug:DESTDIR = ../../build/debug/cleaner
Debug:OBJECTS_DIR = ../../build/debug/cleaner/.obj
Debug:MOC_DIR = ../../build/debug/cleaner/.moc
Debug:RCC_DIR = ../../build/debug/cleaner/.rcc
Debug:UI_DIR = ../../build/debug/cleaner/.ui