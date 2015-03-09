#-------------------------------------------------
#
# Project created by QtCreator 2015-02-08T18:58:35
#
#-------------------------------------------------

QT       += core xml

QT       -= gui

TARGET = civ4_audio_xml_injector
CONFIG   += console qt static
CONFIG   -= app_bundle

TEMPLATE = app

QMAKE_LFLAGS += -static-libgcc -static-libstdc++ -static -lstdc++ -lpthread

SOURCES += main.cpp

DISTFILES += \
    README.md
