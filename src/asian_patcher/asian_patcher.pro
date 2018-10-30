#-------------------------------------------------
#
# Project created by QtCreator 2015-02-27T08:48:54
#
#-------------------------------------------------

QT       += core xml

QT       -= gui

TARGET = asian_patcher
CONFIG   += console qt static
CONFIG   -= app_bundle

TEMPLATE = app

RC_FILE = manifest.rc

QMAKE_LFLAGS += -static-libgcc -static-libstdc++ -static -lstdc++ -lpthread

SOURCES += main.cpp

DISTFILES += \
    README.md

DISTFILES +=

Release:DESTDIR = ../../build/release/asian_patcher
Release:OBJECTS_DIR = ../../build/release/asian_patcher/.obj
Release:MOC_DIR = ../../build/release/asian_patcher/.moc
Release:RCC_DIR = ../../build/release/asian_patcher/.rcc
Release:UI_DIR = ../../build/release/asian_patcher/.ui

Debug:DESTDIR = ../../build/debug/asian_patcher
Debug:OBJECTS_DIR = ../../build/debug/asian_patcher/.obj
Debug:MOC_DIR = ../../build/debug/asian_patcher/.moc
Debug:RCC_DIR = ../../build/debug/asian_patcher/.rcc
Debug:UI_DIR = ../../build/debug/asian_patcher/.ui