QT += core
QT -= gui

QMAKE_LFLAGS = -static -lpthread

CONFIG += c++11

TARGET = checksum
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

RC_FILE = icon.rc

SOURCES += main.cpp

DISTFILES +=

Release:DESTDIR = ../../build/release/checksum
Release:OBJECTS_DIR = ../../build/release/checksum/.obj
Release:MOC_DIR = ../../build/release/checksum/.moc
Release:RCC_DIR = ../../build/release/checksum/.rcc
Release:UI_DIR = ../../build/release/checksum/.ui

Debug:DESTDIR = ../../build/debug/checksum
Debug:OBJECTS_DIR = ../../build/debug/checksum/.obj
Debug:MOC_DIR = ../../build/debug/checksum/.moc
Debug:RCC_DIR = ../../build/debug/checksum/.rcc
Debug:UI_DIR = ../../build/debug/checksum/.ui