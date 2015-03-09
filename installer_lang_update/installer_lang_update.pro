#-------------------------------------------------
#
# Project created by QtCreator 2015-02-23T08:47:52
#
#-------------------------------------------------

QT       += core xml

QT       -= gui

TARGET = installer_lang_update
CONFIG   += console qt static
CONFIG   -= app_bundle

TEMPLATE = app

QMAKE_LFLAGS += -static-libgcc -static-libstdc++ -static -lstdc++ -lpthread

SOURCES += main.cpp

DISTFILES += \
    README.md
