#-------------------------------------------------
#
# Project created by QtCreator 2015-02-08T18:58:35
#
#-------------------------------------------------

QT       += core xml

QT       -= gui

TARGET = audio_injector
CONFIG   += console qt static
CONFIG   -= app_bundle

TEMPLATE = app

QMAKE_LFLAGS += -static-libgcc -static-libstdc++ -static -lstdc++ -lpthread

SOURCES += main.cpp

RC_FILE = icon.rc

DISTFILES += \
    README.md

DISTFILES +=

Release:DESTDIR = ../../build/release/audio_injector
Release:OBJECTS_DIR = ../../build/release/audio_injector/.obj
Release:MOC_DIR = ../../build/release/audio_injector/.moc
Release:RCC_DIR = ../../build/release/audio_injector/.rcc
Release:UI_DIR = ../../build/release/audio_injector/.ui

Debug:DESTDIR = ../../build/debug/audio_injector
Debug:OBJECTS_DIR = ../../build/debug/audio_injector/.obj
Debug:MOC_DIR = ../../build/debug/audio_injector/.moc
Debug:RCC_DIR = ../../build/debug/audio_injector/.rcc
Debug:UI_DIR = ../../build/debug/audio_injector/.ui