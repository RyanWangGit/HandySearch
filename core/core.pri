QT += core sql concurrent
CONFIG += c++11

INCLUDEPATH += $$PWD/include \
    $$PWD/libs/cppjieba/include \
    $$PWD/libs/cppjieba/deps

SOURCES += \
  $$PWD/src/searchcore.cpp

HEADERS += \
    $$PWD/include/searchcore.h \
    $$PWD/src/qjieba.hpp

RESOURCES += $$PWD/core.qrc
