#-------------------------------------------------
#
# Project created by QtCreator 2015-02-07T11:44:53
#
#-------------------------------------------------

QT       += core xml

QT       -= gui

TARGET = and2_transifex_format
CONFIG   += console static
CONFIG   -= app_bundle

TEMPLATE = app

QMAKE_LFLAGS += -static-libgcc -static-libstdc++ -static -lstdc++ -lpthread

SOURCES += main.cpp

DISTFILES += \
    README.md
