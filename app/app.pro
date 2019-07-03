include(../core/core.pri)
TARGET = ../HandySearch # move target one directory up
QT += gui widgets
CONFIG += c++11
TEMPLATE = app

INCLUDEPATH += src/

SOURCES += main.cpp \
    src/handy_search.cpp \
    src/load_ui.cpp

HEADERS += src/handy_search.h \
    src/load_ui.h

FORMS += forms/handy_search.ui \
    forms/load_ui.ui

RESOURCES += app.qrc
