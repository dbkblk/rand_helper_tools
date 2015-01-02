#-------------------------------------------------
#
# Project created by QtCreator 2014-05-22T08:40:05
#
#-------------------------------------------------

INCLUDEPATH += .

QT       += core gui network widgets xml

TARGET = _xml_parser

TEMPLATE = app

RC_FILE = icon.rc

SOURCES += main.cpp \
    w_main.cpp \
    f_lang.cpp \
    f_files.cpp

OTHER_FILES += \
    TODO.md \
    README.md

HEADERS += \
    w_main.h \
    f_lang.h \
    f_files.h

FORMS += \
    w_main.ui
