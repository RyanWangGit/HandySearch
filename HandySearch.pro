TARGET = HandySearch
QT += core gui widgets
CONFIG += c++11
PRECOMPILED_HEADER = ./include/stdafx.h
INCLUDEPATH += ./include
HEADERS += ./include/stdafx.h \
    ./include/HandySearch.h \
    ./include/BloomFilter.h \
    ./include/Dictionary.h \
    ./include/LoadUI.h \
    ./include/WordSegmenter.h \
    ./include/Index.h \
    ./include/Html.h \
    ./include/InvertedList.h \
    ./include/LocalInvertedList.h
SOURCES += ./source/HandySearch.cpp \
    ./source/main.cpp \
    ./source/BloomFilter.cpp \
    ./source/Dictionary.cpp \
    ./source/LoadUI.cpp \
    ./source/Html.cpp \
    ./source/Index.cpp \
    ./source/LocalInvertedList.cpp \
    ./source/InvertedList.cpp \
    ./source/WordSegmenter.cpp
FORMS += ./forms/HandySearch.ui \
    ./forms/LoadUI.ui
RESOURCES += HandySearch.qrc
