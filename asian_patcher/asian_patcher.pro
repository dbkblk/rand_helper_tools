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
