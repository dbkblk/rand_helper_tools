#-------------------------------------------------
#
# Project created by QtCreator 2015-02-18T21:51:56
#
#-------------------------------------------------

QT       += core xml

QT       -= gui

TARGET = art_files_detector
CONFIG   += console qt static
CONFIG   -= app_bundle

TEMPLATE = app

QMAKE_LFLAGS += -static-libgcc -static-libstdc++ -static -lstdc++ -lpthread

RC_FILE = icon.rc

SOURCES += main.cpp

DISTFILES += \
    README.md

DISTFILES +=

Release:DESTDIR = ../../build/release/art_files_detector
Release:OBJECTS_DIR = ../../build/release/art_files_detector/.obj
Release:MOC_DIR = ../../build/release/art_files_detector/.moc
Release:RCC_DIR = ../../build/release/art_files_detector/.rcc
Release:UI_DIR = ../../build/release/art_files_detector/.ui

Debug:DESTDIR = ../../build/debug/art_files_detector
Debug:OBJECTS_DIR = ../../build/debug/art_files_detector/.obj
Debug:MOC_DIR = ../../build/debug/art_files_detector/.moc
Debug:RCC_DIR = ../../build/debug/art_files_detector/.rcc
Debug:UI_DIR = ../../build/debug/art_files_detector/.ui