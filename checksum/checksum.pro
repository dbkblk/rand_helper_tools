QT += core
QT -= gui

QMAKE_LFLAGS = -static -lpthread

CONFIG += c++11

TARGET = checksum
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp
