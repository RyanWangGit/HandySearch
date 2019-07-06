QT += core sql concurrent
CONFIG += c++11

INCLUDEPATH += $$PWD/include \
    $$PWD/libs/cppjieba/include

SOURCES += $$PWD/src/bloom_filter.cpp \
    $$PWD/src/dictionary.cpp \
    $$PWD/src/search_core.cpp \
    $$PWD/src/word_segmenter.cpp

HEADERS += $$PWD/include/bloom_filter.h \
    $$PWD/include/dictionary.h \
    $$PWD/include/search_core.h \
    $$PWD/include/word_segmenter.h

RESOURCES += $$PWD/core.qrc
