QT       += core xml

QT       -= gui

TARGET = exporter
CONFIG   += console qt
CONFIG   -= app_bundle

win32 {
    CONFIG  +=  static
    QMAKE_LFLAGS += -static-libgcc -static-libstdc++ -static -lstdc++ -lpthread
}

SOURCES += main.cpp

RC_FILE = icon.rc

DISTFILES += \
    README.md

DISTFILES +=

Release:DESTDIR = ../../build/release/exporter
Release:OBJECTS_DIR = ../../build/release/exporter/.obj
Release:MOC_DIR = ../../build/release/exporter/.moc
Release:RCC_DIR = ../../build/release/exporter/.rcc
Release:UI_DIR = ../../build/release/exporter/.ui

Debug:DESTDIR = ../../build/debug/exporter
Debug:OBJECTS_DIR = ../../build/debug/exporter/.obj
Debug:MOC_DIR = ../../build/debug/exporter/.moc
Debug:RCC_DIR = ../../build/debug/exporter/.rcc
Debug:UI_DIR = ../../build/debug/exporter/.ui