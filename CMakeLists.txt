cmake_minimum_required(VERSION 3.5)

project(HandySearch
        VERSION 1.0
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)

option(BuildTest "Build the tests." ON)

set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(HANDYSEARCH_LIB handysearch)

add_library(${HANDYSEARCH_LIB} SHARED
        ${INCLUDE_DIR}/handysearch/handysearch.h
        ${INCLUDE_DIR}/handysearch/dictionary.h
        src/handysearch.cpp
        src/dictionary.cpp)

target_include_directories(${HANDYSEARCH_LIB}
        PUBLIC
        $<BUILD_INTERFACE:${INCLUDE_DIR}>)

if(BuildTest)
    ENABLE_TESTING()
    add_subdirectory(tests)
endif()