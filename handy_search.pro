TARGET = HandySearch
QT += core gui widgets
CONFIG += c++11
PRECOMPILED_HEADER = ./core/stdafx.h
INCLUDEPATH += ./core \
    ./ui
HEADERS += ./core/stdafx.h \
    ./ui/handy_search.h \
    ./core/bloom_filter.h \
    ./core/dictionary.h \
    ./ui/load_ui.h \
    ./core/word_segmenter.h \
    ./core/index.h \
    ./core/html.h \
    ./core/inverted_list.h \
    ./core/local_inverted_list.h \
    ./core/search_core.h
SOURCES += ./ui/handy_search.cpp \
    ./core/main.cpp \
    ./core/bloom_filter.cpp \
    ./core/dictionary.cpp \
    ./ui/load_ui.cpp \
    ./core/html.cpp \
    ./core/index.cpp \
    ./core/local_inverted_list.cpp \
    ./core/inverted_list.cpp \
    ./core/word_segmenter.cpp \
    ./core/search_core.cpp
FORMS += ./forms/handy_search.ui \
    ./forms/load_ui.ui
RESOURCES += handy_search.qrc
