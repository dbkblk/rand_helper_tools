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

SOURCES += main.cpp

DISTFILES += \
    README.md
