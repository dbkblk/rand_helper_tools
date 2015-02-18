INCLUDEPATH += .

QT       += core gui network widgets xml

TARGET = xml_exporter

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
