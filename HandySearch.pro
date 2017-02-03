TARGET = HandySearch
QT += core gui widgets
CONFIG += c++11
PRECOMPILED_HEADER = stdafx.h
HEADERS += ./stdafx.h \
    ./HandySearch.h \
    ./Exceptions.h \
    ./HashMap.h \
    ./List.h \
    ./BloomFilter.h \
    ./Dictionary.h \
    ./LoadUI.h \
    ./WordSegmenter.h \
    ./Index.h \
    ./Html.h \
    ./InvertedList.h \
    ./LocalInvertedList.h
SOURCES += ./HandySearch.cpp \
    ./main.cpp \
    ./BloomFilter.cpp \
    ./Dictionary.cpp \
    ./LoadUI.cpp \
    ./Html.cpp \
    ./Index.cpp \
    ./LocalInvertedList.cpp \
    ./InvertedList.cpp \
    ./WordSegmenter.cpp
FORMS += ./HandySearch.ui \
    ./LoadUI.ui
RESOURCES += HandySearch.qrc
