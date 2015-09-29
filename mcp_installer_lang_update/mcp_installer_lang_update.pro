#-------------------------------------------------
#
# Project created by QtCreator 2015-02-23T08:47:52
#
#-------------------------------------------------

QT       += core xml

QT       -= gui

TARGET = script_translations
CONFIG   += console qt static
CONFIG   -= app_bundle

TEMPLATE = app

RC_FILE = manifest.rc

QMAKE_LFLAGS += -static-libgcc -static-libstdc++ -static -lstdc++ -lpthread

SOURCES += main.cpp

DISTFILES += \
    README.md
