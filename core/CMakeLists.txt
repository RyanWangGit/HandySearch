cmake_minimum_required(VERSION 3.5)

find_package(Qt6 COMPONENTS Core Sql Concurrent)
if (NOT Qt6_FOUND)
    find_package(Qt5 5.15 COMPONENTS CoreCore Sql Concurrent REQUIRED)
endif()

add_subdirectory(libs)

set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

include_directories(${INCLUDE_DIR})

set(SearchCore_SRCS ${SOURCE_DIR}/searchcore.cpp)

# manually wrap header file since AUTOMOC doesn't seem to handle files in different folder
qt_wrap_cpp(SearchCore_INCLUDE ${INCLUDE_DIR}/searchcore.h)

add_library(SearchCore SHARED ${SearchCore_INCLUDE} ${SearchCore_SRCS} core.qrc)

if(MSVC)
    target_compile_options(SearchCore PRIVATE /W4)
else()
    target_compile_options(SearchCore PRIVATE -Wall -Wextra -pedantic)
endif()

target_link_libraries(SearchCore PUBLIC cppjieba Qt::Core Qt::Concurrent Qt::Sql)
target_include_directories(SearchCore PUBLIC ${INCLUDE_DIR})
