QT += core xml
QT -= gui

TARGET = scan_tool
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

RC_FILE = icon.rc

SOURCES += main.cpp

DISTFILES +=

Release:DESTDIR = ../../build/release/scan_tool
Release:OBJECTS_DIR = ../../build/release/scan_tool/.obj
Release:MOC_DIR = ../../build/release/scan_tool/.moc
Release:RCC_DIR = ../../build/release/scan_tool/.rcc
Release:UI_DIR = ../../build/release/scan_tool/.ui

Debug:DESTDIR = ../../build/debug/scan_tool
Debug:OBJECTS_DIR = ../../build/debug/scan_tool/.obj
Debug:MOC_DIR = ../../build/debug/scan_tool/.moc
Debug:RCC_DIR = ../../build/debug/scan_tool/.rcc
Debug:UI_DIR = ../../build/debug/scan_tool/.ui