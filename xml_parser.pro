#-------------------------------------------------
#
# Project created by QtCreator 2014-05-22T08:40:05
#
#-------------------------------------------------

QT       += core xml

QT       -= gui

TARGET = xml_parser
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    lib/tinyxml2.cpp

HEADERS += \
    lib/tinyxml2.h

OTHER_FILES += \
    TODO.md
