QT += core sql concurrent
CONFIG += c++11

INCLUDEPATH += $$PWD/include \
    $$PWD/libs/cppjieba/include

SOURCES += \
  $$PWD/src/searchcore.cpp

HEADERS += \
    $$PWD/include/searchcore.h

RESOURCES += $$PWD/core.qrc
