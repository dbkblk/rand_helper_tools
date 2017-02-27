QT       += core xml

QT       -= gui

TARGET = exporter
CONFIG   += console qt
CONFIG   -= app_bundle

TEMPLATE = app

win32 {
  CONFIG  +=  static
  QMAKE_LFLAGS += -static-libgcc -static-libstdc++ -static -lstdc++ -lpthread
}

SOURCES += main.cpp

DISTFILES += \
    README.md
