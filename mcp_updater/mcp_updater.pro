#-------------------------------------------------
#
# Project created by QtCreator 2015-02-11T18:30:43
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = and2_mcp_updater
CONFIG   += console static
CONFIG   -= app_bundle

TEMPLATE = app

QMAKE_LFLAGS += -static-libgcc -static-libstdc++ -static -lstdc++ -lpthread

SOURCES += main.cpp

DISTFILES += \
    README.md
