#-------------------------------------------------
#
# Project created by QtCreator 2015-02-18T21:51:39
#
#-------------------------------------------------

QT       += core xml

QT       -= gui

TARGET = merger
CONFIG   += console qt static
CONFIG   -= app_bundle

TEMPLATE = app

QMAKE_LFLAGS += -static-libgcc -static-libstdc++ -static -lstdc++ -lpthread

SOURCES += main.cpp

DISTFILES += \
    README.md
